#include "Match/RFAMatchManager.h"
#include "Ball/RFABallActor.h"
#include "GameFramework/Character.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Goals/RFAGoalActor.h"
#include "Match/RFAScoreManager.h"
#include "TimerManager.h"

ARFAMatchManager::ARFAMatchManager()
{
    PrimaryActorTick.bCanEverTick = true;
}

void ARFAMatchManager::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    if (!bMatchActive || bWaitingForRestart || bWaitingForKickoff || bMatchFinished)
    {
        return;
    }

    CheckBallRecovery();
    if (!bMatchActive)
    {
        return;
    }

    RemainingTimeSeconds = FMath::Max(0.0f, RemainingTimeSeconds - DeltaSeconds);
    if (RemainingTimeSeconds <= 0.0f)
    {
        EndMatch();
    }
}

void ARFAMatchManager::InitializeMatch(ARFAScoreManager* InScoreManager, ARFABallActor* InBall, const FVector& InBallSpawnLocation)
{
    ScoreManager = InScoreManager;
    Ball = InBall;
    BallSpawnLocation = InBallSpawnLocation;
    RemainingTimeSeconds = MatchDurationSeconds;
    bMatchFinished = false;
    bHasScoringTeam = false;
    bWaitingForRestart = false;
    bWaitingForKickoff = false;

    if (ScoreManager)
    {
        ScoreManager->ResetScore();
    }

    if (Ball)
    {
        Ball->ResetBall(BallSpawnLocation);
    }

    QueueKickoff();
}

void ARFAMatchManager::RegisterGoal(ARFAGoalActor* Goal)
{
    if (!Goal || Goals.Contains(Goal))
    {
        return;
    }

    Goals.Add(Goal);
    Goal->OnGoalScored.AddDynamic(this, &ARFAMatchManager::HandleGoalScored);
}

void ARFAMatchManager::RegisterResetActor(AActor* Actor, const FTransform& ResetTransform)
{
    if (!Actor)
    {
        return;
    }

    ResetActors.Add(Actor);
    ResetTransforms.Add(ResetTransform);
}

void ARFAMatchManager::StartMatch()
{
    bWaitingForRestart = false;
    bWaitingForKickoff = false;
    bMatchActive = RemainingTimeSeconds > 0.0f;
    OnMatchStarted.Broadcast();
}

void ARFAMatchManager::ResetPlayToKickoff()
{
    if (bMatchFinished)
    {
        return;
    }

    GetWorldTimerManager().ClearTimer(GoalResetTimerHandle);
    GetWorldTimerManager().ClearTimer(KickoffTimerHandle);

    if (Ball)
    {
        Ball->ResetBall(BallSpawnLocation);
    }

    ResetRegisteredActors();
    OnPlayReset.Broadcast();
    QueueKickoff();
}

float ARFAMatchManager::GetRestartRemainingTime() const
{
    if (!bWaitingForRestart || !GetWorld())
    {
        return 0.0f;
    }

    return FMath::Max(0.0f, GetWorld()->GetTimerManager().GetTimerRemaining(GoalResetTimerHandle));
}

float ARFAMatchManager::GetKickoffRemainingTime() const
{
    if (!bWaitingForKickoff || !GetWorld())
    {
        return 0.0f;
    }

    return FMath::Max(0.0f, GetWorld()->GetTimerManager().GetTimerRemaining(KickoffTimerHandle));
}

bool ARFAMatchManager::IsMatchTied() const
{
    return ScoreManager && ScoreManager->GetHomeScore() == ScoreManager->GetAwayScore();
}

void ARFAMatchManager::HandleGoalScored(ERedFootTeam ScoringTeam, ARFAGoalActor* Goal, ARFABallActor* ScoredBall)
{
    if (bWaitingForRestart || bWaitingForKickoff || !bMatchActive)
    {
        return;
    }

    if (ScoreManager)
    {
        ScoreManager->AddGoal(ScoringTeam);
    }

    LastScoringTeam = ScoringTeam;
    bHasScoringTeam = true;
    bWaitingForRestart = true;
    OnGoalScored.Broadcast(ScoringTeam, GoalResetDelay);

    if (Ball)
    {
        Ball->StopBall();
    }

    GetWorldTimerManager().SetTimer(GoalResetTimerHandle, this, &ARFAMatchManager::ResetAfterGoal, GoalResetDelay, false);
}

void ARFAMatchManager::ResetAfterGoal()
{
    ResetPlayToKickoff();
}

void ARFAMatchManager::ResetRegisteredActors()
{
    for (int32 Index = 0; Index < ResetActors.Num(); ++Index)
    {
        AActor* Actor = ResetActors[Index];
        if (!Actor || !ResetTransforms.IsValidIndex(Index))
        {
            continue;
        }

        Actor->SetActorTransform(ResetTransforms[Index], false, nullptr, ETeleportType::TeleportPhysics);

        ACharacter* Character = Cast<ACharacter>(Actor);
        if (Character && Character->GetMovementComponent())
        {
            Character->GetMovementComponent()->StopMovementImmediately();
        }

        UPrimitiveComponent* PrimitiveRoot = Cast<UPrimitiveComponent>(Actor->GetRootComponent());
        if (PrimitiveRoot && PrimitiveRoot->IsSimulatingPhysics())
        {
            PrimitiveRoot->SetPhysicsLinearVelocity(FVector::ZeroVector);
            PrimitiveRoot->SetPhysicsAngularVelocityInDegrees(FVector::ZeroVector);
        }
    }
}

void ARFAMatchManager::QueueKickoff()
{
    bMatchActive = false;
    bWaitingForRestart = false;
    bWaitingForKickoff = true;

    if (Ball)
    {
        Ball->StopBall();
    }

    GetWorldTimerManager().SetTimer(KickoffTimerHandle, this, &ARFAMatchManager::StartMatch, KickoffDelay, false);
}

void ARFAMatchManager::CheckBallRecovery()
{
    if (!Ball)
    {
        return;
    }

    const FVector BallLocation = Ball->GetActorLocation();
    const bool bOutOfBounds =
        FMath::Abs(BallLocation.X) > BallRecoveryHalfExtent.X ||
        FMath::Abs(BallLocation.Y) > BallRecoveryHalfExtent.Y ||
        BallLocation.Z < BallRecoveryMinZ;

    if (bOutOfBounds)
    {
        ResetPlayToKickoff();
    }
}

void ARFAMatchManager::EndMatch()
{
    bMatchActive = false;
    bWaitingForKickoff = false;
    bWaitingForRestart = false;
    bMatchFinished = true;

    GetWorldTimerManager().ClearTimer(GoalResetTimerHandle);
    GetWorldTimerManager().ClearTimer(KickoffTimerHandle);

    WinningTeam = ERedFootTeam::Home;
    if (ScoreManager && ScoreManager->GetAwayScore() > ScoreManager->GetHomeScore())
    {
        WinningTeam = ERedFootTeam::Away;
    }

    OnMatchEnded.Broadcast(WinningTeam);
}
