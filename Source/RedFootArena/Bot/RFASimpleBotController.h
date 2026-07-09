#pragma once

#include "CoreMinimal.h"
#include "Core/RFATypes.h"
#include "AIController.h"
#include "RFASimpleBotController.generated.h"

class ARFABallActor;

UCLASS()
class REDFOOTARENA_API ARFASimpleBotController : public AAIController
{
    GENERATED_BODY()

public:
    ARFASimpleBotController();

    virtual void Tick(float DeltaSeconds) override;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bot")
    ERedFootTeam ControlledTeam = ERedFootTeam::Away;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bot")
    float KickDistance = 210.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bot")
    float KickImpulse = 1500.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bot")
    float KickCooldownSeconds = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bot")
    float ApproachOffsetFromBall = 180.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bot")
    float DesiredApproachDistance = 80.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bot")
    float AttackGoalX = -1980.0f;

private:
    ARFABallActor* FindBall() const;
    FVector GetAttackTarget() const;
    FVector GetApproachLocation() const;

    UPROPERTY()
    ARFABallActor* CachedBall;

    float KickCooldownRemaining = 0.0f;
};
