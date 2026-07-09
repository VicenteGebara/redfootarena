#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RFAArenaManager.generated.h"

class ARFAGoalActor;
class USceneComponent;
class UStaticMesh;
class UStaticMeshComponent;

UCLASS()
class REDFOOTARENA_API ARFAArenaManager : public AActor
{
    GENERATED_BODY()

public:
    ARFAArenaManager();

    UFUNCTION(BlueprintCallable, Category = "Arena")
    void BuildArena();

    UFUNCTION(BlueprintPure, Category = "Arena")
    const TArray<ARFAGoalActor*>& GetGoals() const { return SpawnedGoals; }

    UFUNCTION(BlueprintPure, Category = "Arena")
    FTransform GetHomePlayerSpawnTransform() const;

    UFUNCTION(BlueprintPure, Category = "Arena")
    FTransform GetAwayPlayerSpawnTransform() const;

    UFUNCTION(BlueprintPure, Category = "Arena")
    FTransform GetBallSpawnTransform() const;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Arena")
    float ArenaLength = 3600.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Arena")
    float ArenaWidth = 2200.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Arena")
    float WallHeight = 260.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Arena")
    float WallThickness = 40.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Arena")
    float GoalWidth = 680.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Arena")
    float GoalHeight = 240.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Arena")
    float GoalDepth = 180.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Arena")
    TSubclassOf<ARFAGoalActor> GoalActorClass;

private:
    UStaticMeshComponent* CreateArenaPiece(const FString& Name, const FVector& Location, const FVector& Size);
    ARFAGoalActor* SpawnGoal(float XSign);

    UPROPERTY()
    USceneComponent* SceneRoot;

    UPROPERTY()
    UStaticMesh* CubeMesh;

    UPROPERTY()
    TArray<UStaticMeshComponent*> ArenaPieces;

    UPROPERTY()
    TArray<ARFAGoalActor*> SpawnedGoals;
};
