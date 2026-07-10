#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RFABallActor.generated.h"

class UStaticMeshComponent;

UCLASS()
class REDFOOTARENA_API ARFABallActor : public AActor
{
    GENERATED_BODY()

public:
    ARFABallActor();

    UFUNCTION(BlueprintCallable, Category = "Ball")
    void ApplyKick(const FVector& Direction, float ImpulseStrength, AActor* Kicker);

    UFUNCTION(BlueprintCallable, Category = "Ball")
    void ResetBall(const FVector& NewLocation);

    UFUNCTION(BlueprintCallable, Category = "Ball")
    void StopBall();

    UFUNCTION(BlueprintPure, Category = "Ball")
    UStaticMeshComponent* GetBallMesh() const { return BallMesh; }

protected:
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ball")
    UStaticMeshComponent* BallMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ball")
    UStaticMeshComponent* StripeMeshX;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ball")
    UStaticMeshComponent* StripeMeshY;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ball")
    float MaxSpeed = 2800.0f;

private:
    void ApplyVisualColor(UStaticMeshComponent* MeshComponent, const FLinearColor& Color) const;
};
