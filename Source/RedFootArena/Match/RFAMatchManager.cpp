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

    if (!bMatchActive || bWaitingForRestart)
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

    if (ScoreManager)
    {
        ScoreManager->ResetScore();
    }

    if (Ball)
    {
        Ball->ResetBall(BallSpawnLocation);
    }

    StartMatch();
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
    bMatchActive = RemainingTimeSeconds > 0.0f;
    OnMatchStarted.Broadcast();
}

void ARFAMatchManager::HandleGoalScored(ERedFootTeam ScoringTeam, ARFAGoalActor* Goal, ARFABallActor* ScoredBall)
{
    if (bWaitingForRestart || !bMatchActive)
    {
        return;
    }

    if (ScoreManager)
    {
        ScoreManager->AddGoal(ScoringTeam);
    }

    bWaitingForRestart = true;

    if (Ball)
    {
        Ball->StopBall();
    }

    GetWorldTimerManager().SetTimer(GoalResetTimerHandle, this, &ARFAMatchManager::ResetAfterGoal, GoalResetDelay, false);
}

void ARFAMatchManager::ResetAfterGoal()
{
    if (Ball)
    {
        Ball->ResetBall(BallSpawnLocation);
    }

    ResetRegisteredActors();
    StartMatch();
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

void ARFAMatchManager::EndMatch()
{
    bMatchActive = false;

    ERedFootTeam WinningTeam = ERedFootTeam::Home;
    if (ScoreManager && ScoreManager->GetAwayScore() > ScoreManager->GetHomeScore())
    {
        WinningTeam = ERedFootTeam::Away;
    }

    OnMatchEnded.Broadcast(WinningTeam);
}
