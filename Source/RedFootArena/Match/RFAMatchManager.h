#pragma once

#include "CoreMinimal.h"
#include "Core/RFATypes.h"
#include "GameFramework/Actor.h"
#include "RFAMatchManager.generated.h"

class ARFABallActor;
class ARFAGoalActor;
class ARFAScoreManager;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FRFAOnMatchStarted);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FRFAOnPlayReset);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRFAOnMatchEnded, ERedFootTeam, WinningTeam);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FRFAOnMatchGoalScored, ERedFootTeam, ScoringTeam, float, RestartDelay);

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

    UFUNCTION(BlueprintCallable, Category = "Match")
    void ResetPlayToKickoff();

    UFUNCTION(BlueprintPure, Category = "Match")
    float GetRemainingTime() const { return RemainingTimeSeconds; }

    UFUNCTION(BlueprintPure, Category = "Match")
    float GetMatchDuration() const { return MatchDurationSeconds; }

    UFUNCTION(BlueprintPure, Category = "Match")
    bool IsMatchActive() const { return bMatchActive; }

    UFUNCTION(BlueprintPure, Category = "Match")
    bool IsWaitingForRestart() const { return bWaitingForRestart; }

    UFUNCTION(BlueprintPure, Category = "Match")
    bool IsWaitingForKickoff() const { return bWaitingForKickoff; }

    UFUNCTION(BlueprintPure, Category = "Match")
    bool IsMatchFinished() const { return bMatchFinished; }

    UFUNCTION(BlueprintPure, Category = "Match")
    bool HasScoringTeam() const { return bHasScoringTeam; }

    UFUNCTION(BlueprintPure, Category = "Match")
    ERedFootTeam GetLastScoringTeam() const { return LastScoringTeam; }

    UFUNCTION(BlueprintPure, Category = "Match")
    ERedFootTeam GetWinningTeam() const { return WinningTeam; }

    UFUNCTION(BlueprintPure, Category = "Match")
    bool IsMatchTied() const;

    UFUNCTION(BlueprintPure, Category = "Match")
    float GetRestartRemainingTime() const;

    UFUNCTION(BlueprintPure, Category = "Match")
    float GetKickoffRemainingTime() const;

    UFUNCTION(BlueprintPure, Category = "Match")
    ARFAScoreManager* GetScoreManager() const { return ScoreManager; }

    UPROPERTY(BlueprintAssignable, Category = "Match")
    FRFAOnMatchStarted OnMatchStarted;

    UPROPERTY(BlueprintAssignable, Category = "Match")
    FRFAOnPlayReset OnPlayReset;

    UPROPERTY(BlueprintAssignable, Category = "Match")
    FRFAOnMatchEnded OnMatchEnded;

    UPROPERTY(BlueprintAssignable, Category = "Match")
    FRFAOnMatchGoalScored OnGoalScored;

protected:
    UFUNCTION()
    void HandleGoalScored(ERedFootTeam ScoringTeam, ARFAGoalActor* Goal, ARFABallActor* ScoredBall);

    void ResetAfterGoal();
    void ResetRegisteredActors();
    void QueueKickoff();
    void CheckBallRecovery();
    void EndMatch();

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Match")
    float MatchDurationSeconds = 180.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Match")
    float GoalResetDelay = 1.5f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Match")
    float KickoffDelay = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Match|Recovery")
    FVector2D BallRecoveryHalfExtent = FVector2D(2600.0f, 1600.0f);

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Match|Recovery")
    float BallRecoveryMinZ = -260.0f;

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
    FTimerHandle KickoffTimerHandle;

    FVector BallSpawnLocation = FVector::ZeroVector;
    float RemainingTimeSeconds = 0.0f;
    ERedFootTeam LastScoringTeam = ERedFootTeam::Home;
    ERedFootTeam WinningTeam = ERedFootTeam::Home;
    bool bMatchActive = false;
    bool bWaitingForRestart = false;
    bool bWaitingForKickoff = false;
    bool bMatchFinished = false;
    bool bHasScoringTeam = false;
};
