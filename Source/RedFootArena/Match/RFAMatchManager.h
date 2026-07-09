#pragma once

#include "CoreMinimal.h"
#include "Core/RFATypes.h"
#include "GameFramework/Actor.h"
#include "RFAMatchManager.generated.h"

class ARFABallActor;
class ARFAGoalActor;
class ARFAScoreManager;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FRFAOnMatchStarted);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRFAOnMatchEnded, ERedFootTeam, WinningTeam);

UCLASS()
class REDFOOTARENA_API ARFAMatchManager : public AActor
{
    GENERATED_BODY()

public:
    ARFAMatchManager();

    virtual void Tick(float DeltaSeconds) override;

    UFUNCTION(BlueprintCallable, Category = "Match")
    void InitializeMatch(ARFAScoreManager* InScoreManager, ARFABallActor* InBall, const FVector& InBallSpawnLocation);

    UFUNCTION(BlueprintCallable, Category = "Match")
    void RegisterGoal(ARFAGoalActor* Goal);

    UFUNCTION(BlueprintCallable, Category = "Match")
    void RegisterResetActor(AActor* Actor, const FTransform& ResetTransform);

    UFUNCTION(BlueprintCallable, Category = "Match")
    void StartMatch();

    UFUNCTION(BlueprintPure, Category = "Match")
    float GetRemainingTime() const { return RemainingTimeSeconds; }

    UFUNCTION(BlueprintPure, Category = "Match")
    float GetMatchDuration() const { return MatchDurationSeconds; }

    UFUNCTION(BlueprintPure, Category = "Match")
    bool IsMatchActive() const { return bMatchActive; }

    UFUNCTION(BlueprintPure, Category = "Match")
    ARFAScoreManager* GetScoreManager() const { return ScoreManager; }

    UPROPERTY(BlueprintAssignable, Category = "Match")
    FRFAOnMatchStarted OnMatchStarted;

    UPROPERTY(BlueprintAssignable, Category = "Match")
    FRFAOnMatchEnded OnMatchEnded;

protected:
    UFUNCTION()
    void HandleGoalScored(ERedFootTeam ScoringTeam, ARFAGoalActor* Goal, ARFABallActor* ScoredBall);

    void ResetAfterGoal();
    void ResetRegisteredActors();
    void EndMatch();

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Match")
    float MatchDurationSeconds = 180.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Match")
    float GoalResetDelay = 1.5f;

private:
    UPROPERTY()
    ARFAScoreManager* ScoreManager;

    UPROPERTY()
    ARFABallActor* Ball;

    UPROPERTY()
    TArray<ARFAGoalActor*> Goals;

    UPROPERTY()
    TArray<AActor*> ResetActors;

    UPROPERTY()
    TArray<FTransform> ResetTransforms;

    FTimerHandle GoalResetTimerHandle;

    FVector BallSpawnLocation = FVector::ZeroVector;
    float RemainingTimeSeconds = 0.0f;
    bool bMatchActive = false;
    bool bWaitingForRestart = false;
};

