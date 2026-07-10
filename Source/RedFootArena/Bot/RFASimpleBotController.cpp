#include "Bot/RFASimpleBotController.h"
#include "Ball/RFABallActor.h"
#include "EngineUtils.h"
#include "GameFramework/Pawn.h"
#include "Match/RFAMatchManager.h"

ARFASimpleBotController::ARFASimpleBotController()
{
    PrimaryActorTick.bCanEverTick = true;
}

void ARFASimpleBotController::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    APawn* ControlledPawn = GetPawn();
    if (!ControlledPawn)
    {
        return;
    }

    if (!CanPlayBall())
    {
        return;
    }

    KickCooldownRemaining = FMath::Max(0.0f, KickCooldownRemaining - DeltaSeconds);

    if (!CachedBall)
    {
        CachedBall = FindBall();
    }

    if (!CachedBall)
    {
        return;
    }

    const FVector BallLocation = CachedBall->GetActorLocation();
    FVector ToBall = BallLocation - ControlledPawn->GetActorLocation();
    ToBall.Z = 0.0f;

    const float DistanceToBall = ToBall.Size();
    if (DistanceToBall > KickDistance)
    {
        FVector ToApproachLocation = GetApproachLocation() - ControlledPawn->GetActorLocation();
        ToApproachLocation.Z = 0.0f;

        const FVector MoveDirection = ToApproachLocation.Size() > DesiredApproachDistance
            ? ToApproachLocation.GetSafeNormal()
            : ToBall.GetSafeNormal();

        ControlledPawn->AddMovementInput(MoveDirection, 1.0f);
        ControlledPawn->SetActorRotation(MoveDirection.Rotation());
        return;
    }

    if (KickCooldownRemaining > 0.0f)
    {
        return;
    }

    FVector KickDirection = GetAttackTarget() - CachedBall->GetActorLocation();
    KickDirection.Z = 0.08f;
    CachedBall->ApplyKick(KickDirection, KickImpulse, ControlledPawn);
    KickCooldownRemaining = KickCooldownSeconds;
}

bool ARFASimpleBotController::CanPlayBall() const
{
    if (!GetWorld())
    {
        return false;
    }

    for (TActorIterator<ARFAMatchManager> It(GetWorld()); It; ++It)
    {
        return It->IsMatchActive();
    }

    return true;
}

ARFABallActor* ARFASimpleBotController::FindBall() const
{
    if (!GetWorld())
    {
        return nullptr;
    }

    for (TActorIterator<ARFABallActor> It(GetWorld()); It; ++It)
    {
        return *It;
    }

    return nullptr;
}

FVector ARFASimpleBotController::GetAttackTarget() const
{
    const float TargetX = ControlledTeam == ERedFootTeam::Away ? AttackGoalX : -AttackGoalX;
    return FVector(TargetX, 0.0f, 90.0f);
}

FVector ARFASimpleBotController::GetApproachLocation() const
{
    if (!CachedBall)
    {
        return FVector::ZeroVector;
    }

    FVector BallToGoal = GetAttackTarget() - CachedBall->GetActorLocation();
    BallToGoal.Z = 0.0f;
    return CachedBall->GetActorLocation() - BallToGoal.GetSafeNormal() * ApproachOffsetFromBall;
}
