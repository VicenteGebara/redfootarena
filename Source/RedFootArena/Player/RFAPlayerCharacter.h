#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RFAPlayerCharacter.generated.h"

class ARFABallActor;
class UCameraComponent;
class UInputComponent;
class URFAPlayerStatsComponent;
class URFAStaminaComponent;
class USpringArmComponent;
class UStaticMeshComponent;

UCLASS()
class REDFOOTARENA_API ARFAPlayerCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    ARFAPlayerCharacter();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaSeconds) override;
    virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

    UFUNCTION(BlueprintPure, Category = "Player")
    FVector GetAimDirection() const;

    UFUNCTION(BlueprintCallable, Category = "Player|Ball")
    void KickBall();

    UFUNCTION(BlueprintCallable, Category = "Player|Ball")
    void PassBall();

    UFUNCTION(BlueprintCallable, Category = "Player")
    void RequestPlayReset();

    UFUNCTION(BlueprintCallable, Category = "Player")
    void ResetArcadeVelocity();

    UFUNCTION(BlueprintPure, Category = "Player")
    URFAPlayerStatsComponent* GetStatsComponent() const { return StatsComponent; }

    UFUNCTION(BlueprintPure, Category = "Player")
    URFAStaminaComponent* GetStaminaComponent() const { return StaminaComponent; }

protected:
    void MoveForward(float Value);
    void MoveRight(float Value);
    void Turn(float Value);
    void LookUp(float Value);
    void StartSprint();
    void StopSprint();
    void ApplyBallAction(float Strength, float Lift);

    ARFABallActor* FindKickableBall() const;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
    USpringArmComponent* CameraBoom;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
    UCameraComponent* FollowCamera;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Visual")
    UStaticMeshComponent* BodyMesh;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Visual")
    FLinearColor BodyColor = FLinearColor(0.9f, 0.12f, 0.1f, 1.0f);

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    URFAStaminaComponent* StaminaComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    URFAPlayerStatsComponent* StatsComponent;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ball")
    float BallActionRange = 185.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ball")
    float BallActionRadius = 115.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ball")
    float KickLift = 0.16f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ball")
    float PassLift = 0.05f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ball")
    float BallActionCooldown = 0.18f;

private:
    float BallActionCooldownRemaining = 0.0f;
};
