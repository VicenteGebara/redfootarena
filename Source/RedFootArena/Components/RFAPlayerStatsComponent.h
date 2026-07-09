#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RFAPlayerStatsComponent.generated.h"

UCLASS(ClassGroup = (RedFootArena), meta = (BlueprintSpawnableComponent))
class REDFOOTARENA_API URFAPlayerStatsComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    URFAPlayerStatsComponent();

    UFUNCTION(BlueprintPure, Category = "Stats")
    float GetMoveSpeed() const { return MoveSpeed; }

    UFUNCTION(BlueprintPure, Category = "Stats")
    float GetSprintSpeed() const { return SprintSpeed; }

    UFUNCTION(BlueprintPure, Category = "Stats")
    float GetKickPower() const { return KickPower; }

    UFUNCTION(BlueprintPure, Category = "Stats")
    float GetPassPower() const { return PassPower; }

    UFUNCTION(BlueprintPure, Category = "Stats")
    float GetMaxStamina() const { return MaxStamina; }

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats|Movement")
    float MoveSpeed = 680.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats|Movement")
    float SprintSpeed = 980.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats|Ball")
    float KickPower = 1850.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats|Ball")
    float PassPower = 1050.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats|Stamina")
    float MaxStamina = 100.0f;
};

