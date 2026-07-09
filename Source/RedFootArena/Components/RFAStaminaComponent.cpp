#include "Components/RFAStaminaComponent.h"

URFAStaminaComponent::URFAStaminaComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void URFAStaminaComponent::BeginPlay()
{
    Super::BeginPlay();
    SetCurrentStamina(MaxStamina);
}

void URFAStaminaComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (bWantsSprint && CanSprint())
    {
        SetCurrentStamina(CurrentStamina - SprintDrainPerSecond * DeltaTime);
        return;
    }

    if (!bWantsSprint && CurrentStamina < MaxStamina)
    {
        SetCurrentStamina(CurrentStamina + RegenPerSecond * DeltaTime);
    }
}

void URFAStaminaComponent::SetWantsSprint(bool bNewWantsSprint)
{
    bWantsSprint = bNewWantsSprint;
}

void URFAStaminaComponent::SetMaxStamina(float NewMaxStamina)
{
    MaxStamina = FMath::Max(1.0f, NewMaxStamina);
    SetCurrentStamina(MaxStamina);
}

bool URFAStaminaComponent::CanSprint() const
{
    return CurrentStamina > SprintStopThreshold;
}

bool URFAStaminaComponent::IsSprinting() const
{
    return bWantsSprint && CanSprint();
}

void URFAStaminaComponent::SetCurrentStamina(float NewStamina)
{
    const float ClampedStamina = FMath::Clamp(NewStamina, 0.0f, MaxStamina);
    if (!FMath::IsNearlyEqual(CurrentStamina, ClampedStamina))
    {
        CurrentStamina = ClampedStamina;
        OnStaminaChanged.Broadcast(CurrentStamina, MaxStamina);
    }
}

