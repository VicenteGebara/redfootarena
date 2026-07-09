#pragma once

#include "CoreMinimal.h"
#include "Core/RFATypes.h"
#include "GameFramework/Info.h"
#include "RFAScoreManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FRFAOnScoreChanged, int32, HomeScore, int32, AwayScore);

UCLASS()
class REDFOOTARENA_API ARFAScoreManager : public AInfo
{
    GENERATED_BODY()

public:
    ARFAScoreManager();

    UFUNCTION(BlueprintCallable, Category = "Score")
    void AddGoal(ERedFootTeam ScoringTeam);

    UFUNCTION(BlueprintCallable, Category = "Score")
    void ResetScore();

    UFUNCTION(BlueprintPure, Category = "Score")
    int32 GetHomeScore() const { return HomeScore; }

    UFUNCTION(BlueprintPure, Category = "Score")
    int32 GetAwayScore() const { return AwayScore; }

    UPROPERTY(BlueprintAssignable, Category = "Score")
    FRFAOnScoreChanged OnScoreChanged;

private:
    UPROPERTY(VisibleAnywhere, Category = "Score")
    int32 HomeScore = 0;

    UPROPERTY(VisibleAnywhere, Category = "Score")
    int32 AwayScore = 0;
};

