#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RFAStaminaComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FRFAOnStaminaChanged, float, CurrentStamina, float, MaxStamina);

UCLASS(ClassGroup = (RedFootArena), meta = (BlueprintSpawnableComponent))
class REDFOOTARENA_API URFAStaminaComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    URFAStaminaComponent();

    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    UFUNCTION(BlueprintCallable, Category = "Stamina")
    void SetWantsSprint(bool bNewWantsSprint);

    UFUNCTION(BlueprintCallable, Category = "Stamina")
    void SetMaxStamina(float NewMaxStamina);

    UFUNCTION(BlueprintPure, Category = "Stamina")
    bool CanSprint() const;

    UFUNCTION(BlueprintPure, Category = "Stamina")
    bool IsSprinting() const;

    UFUNCTION(BlueprintPure, Category = "Stamina")
    float GetCurrentStamina() const { return CurrentStamina; }

    UFUNCTION(BlueprintPure, Category = "Stamina")
    float GetMaxStamina() const { return MaxStamina; }

    UPROPERTY(BlueprintAssignable, Category = "Stamina")
    FRFAOnStaminaChanged OnStaminaChanged;

private:
    void SetCurrentStamina(float NewStamina);

    UPROPERTY(EditAnywhere, Category = "Stamina")
    float MaxStamina = 100.0f;

    UPROPERTY(EditAnywhere, Category = "Stamina")
    float SprintDrainPerSecond = 28.0f;

    UPROPERTY(EditAnywhere, Category = "Stamina")
    float RegenPerSecond = 22.0f;

    UPROPERTY(EditAnywhere, Category = "Stamina")
    float SprintStopThreshold = 2.0f;

    UPROPERTY(VisibleAnywhere, Category = "Stamina")
    float CurrentStamina = 100.0f;

    bool bWantsSprint = false;
};

