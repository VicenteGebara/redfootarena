#pragma once

#include "CoreMinimal.h"
#include "Core/RFATypes.h"
#include "GameFramework/Actor.h"
#include "RFAGoalActor.generated.h"

class ARFABallActor;
class UBoxComponent;
class USceneComponent;
class UStaticMeshComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FRFAOnGoalScoredSignature, ERedFootTeam, ScoringTeam, ARFAGoalActor*, Goal, ARFABallActor*, Ball);

UCLASS()
class REDFOOTARENA_API ARFAGoalActor : public AActor
{
    GENERATED_BODY()

public:
    ARFAGoalActor();

    UFUNCTION(BlueprintCallable, Category = "Goal")
    void ConfigureGoal(ERedFootTeam InDefendingTeam, float InGoalWidth, float InGoalHeight, float InGoalDepth);

    UFUNCTION(BlueprintPure, Category = "Goal")
    ERedFootTeam GetDefendingTeam() const { return DefendingTeam; }

    UPROPERTY(BlueprintAssignable, Category = "Goal")
    FRFAOnGoalScoredSignature OnGoalScored;

protected:
    UFUNCTION()
    void HandleGoalOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Goal")
    USceneComponent* SceneRoot;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Goal")
    UBoxComponent* GoalTrigger;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Goal")
    UStaticMeshComponent* LeftPost;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Goal")
    UStaticMeshComponent* RightPost;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Goal")
    UStaticMeshComponent* Crossbar;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Goal")
    ERedFootTeam DefendingTeam = ERedFootTeam::Home;
};
