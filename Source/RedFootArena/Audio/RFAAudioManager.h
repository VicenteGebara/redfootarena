#pragma once

#include "CoreMinimal.h"
#include "Core/RFATypes.h"
#include "GameFramework/Actor.h"
#include "RFAAudioManager.generated.h"

class USoundBase;

UCLASS()
class REDFOOTARENA_API ARFAAudioManager : public AActor
{
    GENERATED_BODY()

public:
    ARFAAudioManager();

    UFUNCTION(BlueprintCallable, Category = "Audio")
    void PlayKick();

    UFUNCTION(BlueprintCallable, Category = "Audio")
    void PlayPass();

    UFUNCTION(BlueprintCallable, Category = "Audio")
    void PlayGoal();

    UFUNCTION(BlueprintCallable, Category = "Audio")
    void PlayKickoff();

    UFUNCTION(BlueprintCallable, Category = "Audio")
    void PlayReset();

    UFUNCTION(BlueprintCallable, Category = "Audio")
    void PlayMatchEnd();

    UFUNCTION()
    void HandleGoalScored(ERedFootTeam ScoringTeam, float RestartDelay);

    UFUNCTION()
    void HandleMatchStarted();

    UFUNCTION()
    void HandleMatchEnded(ERedFootTeam WinningTeam);

    UFUNCTION()
    void HandlePlayReset();

private:
    void PlaySound2D(USoundBase* Sound, float VolumeMultiplier, float PitchMultiplier) const;
    void PlayPing(float VolumeMultiplier, float PitchMultiplier) const;

    UPROPERTY()
    USoundBase* PingSound;

};
