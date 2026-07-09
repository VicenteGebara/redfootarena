#include "Bot/RFASimpleBotController.h"
#include "Ball/RFABallActor.h"
#include "EngineUtils.h"
#include "GameFramework/Pawn.h"

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

    KickCooldownRemaining = FMath::Max(0.0f, KickCooldownRemaining - DeltaSeconds);

    if (!CachedBall)
    {
        CachedBall = FindBall();
    }

    if (!CachedBall)
    {
        return;
    }

    FVector ToBall = CachedBall->GetActorLocation() - ControlledPawn->GetActorLocation();
    ToBall.Z = 0.0f;

    const float DistanceToBall = ToBall.Size();
    if (DistanceToBall > KickDistance)
    {
        const FVector MoveDirection = ToBall.GetSafeNormal();
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

