#include "Player/RFAPlayerCharacter.h"
#include "Ball/RFABallActor.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/RFAPlayerStatsComponent.h"
#include "Components/RFAStaminaComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/OverlapResult.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "UObject/ConstructorHelpers.h"

ARFAPlayerCharacter::ARFAPlayerCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f);

    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
    GetCharacterMovement()->MaxAcceleration = 4200.0f;
    GetCharacterMovement()->BrakingDecelerationWalking = 5200.0f;
    GetCharacterMovement()->GroundFriction = 8.0f;
    GetCharacterMovement()->MaxWalkSpeed = 680.0f;

    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->TargetArmLength = 460.0f;
    CameraBoom->SocketOffset = FVector(0.0f, 75.0f, 85.0f);
    CameraBoom->bUsePawnControlRotation = true;

    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    FollowCamera->bUsePawnControlRotation = false;

    BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMesh"));
    BodyMesh->SetupAttachment(RootComponent);
    BodyMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    BodyMesh->SetRelativeLocation(FVector(0.0f, 0.0f, -35.0f));
    BodyMesh->SetRelativeScale3D(FVector(0.55f, 0.55f, 1.45f));

    static ConstructorHelpers::FObjectFinder<UStaticMesh> BodyAsset(TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
    if (BodyAsset.Succeeded())
    {
        BodyMesh->SetStaticMesh(BodyAsset.Object);
    }

    StaminaComponent = CreateDefaultSubobject<URFAStaminaComponent>(TEXT("StaminaComponent"));
    StatsComponent = CreateDefaultSubobject<URFAPlayerStatsComponent>(TEXT("StatsComponent"));
}

void ARFAPlayerCharacter::BeginPlay()
{
    Super::BeginPlay();

    if (StatsComponent && StaminaComponent)
    {
        StaminaComponent->SetMaxStamina(StatsComponent->GetMaxStamina());
    }

    if (BodyMesh)
    {
        if (UMaterialInstanceDynamic* DynamicMaterial = BodyMesh->CreateAndSetMaterialInstanceDynamic(0))
        {
            DynamicMaterial->SetVectorParameterValue(TEXT("Color"), BodyColor);
        }
    }
}

void ARFAPlayerCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    BallActionCooldownRemaining = FMath::Max(0.0f, BallActionCooldownRemaining - DeltaSeconds);

    if (!StatsComponent || !StaminaComponent)
    {
        return;
    }

    GetCharacterMovement()->MaxWalkSpeed = StaminaComponent->IsSprinting()
        ? StatsComponent->GetSprintSpeed()
        : StatsComponent->GetMoveSpeed();
}

void ARFAPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ARFAPlayerCharacter::MoveForward);
    PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &ARFAPlayerCharacter::MoveRight);
    PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ARFAPlayerCharacter::Turn);
    PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &ARFAPlayerCharacter::LookUp);

    PlayerInputComponent->BindAction(TEXT("Sprint"), IE_Pressed, this, &ARFAPlayerCharacter::StartSprint);
    PlayerInputComponent->BindAction(TEXT("Sprint"), IE_Released, this, &ARFAPlayerCharacter::StopSprint);
    PlayerInputComponent->BindAction(TEXT("Kick"), IE_Pressed, this, &ARFAPlayerCharacter::KickBall);
    PlayerInputComponent->BindAction(TEXT("Pass"), IE_Pressed, this, &ARFAPlayerCharacter::PassBall);
}

FVector ARFAPlayerCharacter::GetAimDirection() const
{
    if (Controller)
    {
        const FRotator ControlYaw(0.0f, Controller->GetControlRotation().Yaw, 0.0f);
        return FRotationMatrix(ControlYaw).GetUnitAxis(EAxis::X);
    }

    return GetActorForwardVector();
}

void ARFAPlayerCharacter::KickBall()
{
    if (!StatsComponent)
    {
        return;
    }

    ApplyBallAction(StatsComponent->GetKickPower(), KickLift);
}

void ARFAPlayerCharacter::PassBall()
{
    if (!StatsComponent)
    {
        return;
    }

    ApplyBallAction(StatsComponent->GetPassPower(), PassLift);
}

void ARFAPlayerCharacter::ResetArcadeVelocity()
{
    if (GetCharacterMovement())
    {
        GetCharacterMovement()->StopMovementImmediately();
    }

    if (StaminaComponent)
    {
        StaminaComponent->SetWantsSprint(false);
    }
}

void ARFAPlayerCharacter::MoveForward(float Value)
{
    if (FMath::IsNearlyZero(Value))
    {
        return;
    }

    const FVector Direction = GetAimDirection();
    AddMovementInput(Direction, Value);
}

void ARFAPlayerCharacter::MoveRight(float Value)
{
    if (FMath::IsNearlyZero(Value))
    {
        return;
    }

    if (!Controller)
    {
        AddMovementInput(GetActorRightVector(), Value);
        return;
    }

    const FRotator ControlYaw(0.0f, Controller->GetControlRotation().Yaw, 0.0f);
    const FVector Direction = FRotationMatrix(ControlYaw).GetUnitAxis(EAxis::Y);
    AddMovementInput(Direction, Value);
}

void ARFAPlayerCharacter::Turn(float Value)
{
    AddControllerYawInput(Value);
}

void ARFAPlayerCharacter::LookUp(float Value)
{
    AddControllerPitchInput(Value);
}

void ARFAPlayerCharacter::StartSprint()
{
    if (StaminaComponent && StaminaComponent->CanSprint())
    {
        StaminaComponent->SetWantsSprint(true);
    }
}

void ARFAPlayerCharacter::StopSprint()
{
    if (StaminaComponent)
    {
        StaminaComponent->SetWantsSprint(false);
    }
}

void ARFAPlayerCharacter::ApplyBallAction(float Strength, float Lift)
{
    if (BallActionCooldownRemaining > 0.0f)
    {
        return;
    }

    ARFABallActor* Ball = FindKickableBall();
    if (!Ball)
    {
        return;
    }

    FVector Direction = GetAimDirection();
    Direction.Z = Lift;
    Ball->ApplyKick(Direction, Strength, this);
    BallActionCooldownRemaining = BallActionCooldown;
}

ARFABallActor* ARFAPlayerCharacter::FindKickableBall() const
{
    if (!GetWorld())
    {
        return nullptr;
    }

    const FVector SearchLocation = GetActorLocation() + GetAimDirection() * BallActionRange;
    const FCollisionShape SearchShape = FCollisionShape::MakeSphere(BallActionRadius);
    FCollisionObjectQueryParams ObjectQueryParams;
    ObjectQueryParams.AddObjectTypesToQuery(ECC_PhysicsBody);

    TArray<FOverlapResult> Overlaps;
    GetWorld()->OverlapMultiByObjectType(Overlaps, SearchLocation, FQuat::Identity, ObjectQueryParams, SearchShape);
    if (Overlaps.IsEmpty())
    {
        GetWorld()->OverlapMultiByObjectType(Overlaps, GetActorLocation(), FQuat::Identity, ObjectQueryParams, SearchShape);
    }

    ARFABallActor* BestBall = nullptr;
    float BestDistanceSquared = TNumericLimits<float>::Max();
    for (const FOverlapResult& Overlap : Overlaps)
    {
        ARFABallActor* CandidateBall = Cast<ARFABallActor>(Overlap.GetActor());
        if (!CandidateBall)
        {
            continue;
        }

        const float DistanceSquared = FVector::DistSquared(GetActorLocation(), CandidateBall->GetActorLocation());
        if (DistanceSquared < BestDistanceSquared)
        {
            BestDistanceSquared = DistanceSquared;
            BestBall = CandidateBall;
        }
    }

    return BestBall;
}
