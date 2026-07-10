#include "Player/RFAPlayerCharacter.h"
#include "Audio/RFAAudioManager.h"
#include "Ball/RFABallActor.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/RFAPlayerStatsComponent.h"
#include "Components/RFAStaminaComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/SkeletalMesh.h"
#include "Engine/OverlapResult.h"
#include "EngineUtils.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Match/RFAMatchManager.h"
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
    CameraBoom->TargetArmLength = 540.0f;
    CameraBoom->SocketOffset = FVector(0.0f, 85.0f, 120.0f);
    CameraBoom->bUsePawnControlRotation = true;

    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    FollowCamera->bUsePawnControlRotation = false;
    FollowCamera->SetFieldOfView(82.0f);

    // The imported Kenney character is the visible player. The inherited
    // skeletal mesh also gives us a clean path to add animations later.
    static ConstructorHelpers::FObjectFinder<USkeletalMesh> HumanAsset(
        TEXT("/Game/External/Kenney/MiniCharacters/character-male-a.character-male-a"));
    if (HumanAsset.Succeeded())
    {
        GetMesh()->SetSkeletalMesh(HumanAsset.Object);
        GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -96.0f));
        GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
        GetMesh()->SetRelativeScale3D(FVector(1.0f));
        GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        GetMesh()->SetCastShadow(true);
    }

    BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMesh"));
    BodyMesh->SetupAttachment(RootComponent);
    BodyMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    BodyMesh->SetHiddenInGame(true);
    BodyMesh->SetRelativeLocation(FVector(0.0f, 0.0f, -35.0f));
    BodyMesh->SetRelativeScale3D(FVector(0.55f, 0.55f, 1.45f));

    HeadMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HeadMesh"));
    HeadMesh->SetupAttachment(RootComponent);
    HeadMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    HeadMesh->SetHiddenInGame(true);
    HeadMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 54.0f));
    HeadMesh->SetRelativeScale3D(FVector(0.32f));

    TeamRingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TeamRingMesh"));
    TeamRingMesh->SetupAttachment(RootComponent);
    TeamRingMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    TeamRingMesh->SetRelativeLocation(FVector(0.0f, 0.0f, -92.0f));
    TeamRingMesh->SetRelativeScale3D(FVector(0.95f, 0.95f, 0.04f));

    ForwardMarkerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ForwardMarkerMesh"));
    ForwardMarkerMesh->SetupAttachment(RootComponent);
    ForwardMarkerMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    ForwardMarkerMesh->SetHiddenInGame(true);
    ForwardMarkerMesh->SetRelativeLocation(FVector(58.0f, 0.0f, -72.0f));
    ForwardMarkerMesh->SetRelativeScale3D(FVector(0.72f, 0.16f, 0.06f));

    static ConstructorHelpers::FObjectFinder<UStaticMesh> BodyAsset(TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
    if (BodyAsset.Succeeded())
    {
        BodyMesh->SetStaticMesh(BodyAsset.Object);
        TeamRingMesh->SetStaticMesh(BodyAsset.Object);
    }

    static ConstructorHelpers::FObjectFinder<UStaticMesh> HeadAsset(TEXT("/Engine/BasicShapes/Sphere.Sphere"));
    if (HeadAsset.Succeeded())
    {
        HeadMesh->SetStaticMesh(HeadAsset.Object);
    }

    static ConstructorHelpers::FObjectFinder<UStaticMesh> MarkerAsset(TEXT("/Engine/BasicShapes/Cube.Cube"));
    if (MarkerAsset.Succeeded())
    {
        ForwardMarkerMesh->SetStaticMesh(MarkerAsset.Object);
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

    ApplyVisualColor(BodyMesh, BodyColor);
    ApplyVisualColor(HeadMesh, FLinearColor(0.98f, 0.78f, 0.56f, 1.0f));
    ApplyVisualColor(TeamRingMesh, AccentColor);
    ApplyVisualColor(ForwardMarkerMesh, AccentColor);
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
    PlayerInputComponent->BindAction(TEXT("ResetPlay"), IE_Pressed, this, &ARFAPlayerCharacter::RequestPlayReset);
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

    if (ApplyBallAction(StatsComponent->GetKickPower(), KickLift))
    {
        PlayBallActionSound(true);
    }
}

void ARFAPlayerCharacter::PassBall()
{
    if (!StatsComponent)
    {
        return;
    }

    if (ApplyBallAction(StatsComponent->GetPassPower(), PassLift))
    {
        PlayBallActionSound(false);
    }
}

void ARFAPlayerCharacter::RequestPlayReset()
{
    if (!GetWorld())
    {
        return;
    }

    for (TActorIterator<ARFAMatchManager> It(GetWorld()); It; ++It)
    {
        It->ResetPlayToKickoff();
        return;
    }
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

bool ARFAPlayerCharacter::ApplyBallAction(float Strength, float Lift)
{
    if (!GetWorld())
    {
        return false;
    }

    for (TActorIterator<ARFAMatchManager> It(GetWorld()); It; ++It)
    {
        if (!It->IsMatchActive())
        {
            return false;
        }

        break;
    }

    if (BallActionCooldownRemaining > 0.0f)
    {
        return false;
    }

    ARFABallActor* Ball = FindKickableBall();
    if (!Ball)
    {
        return false;
    }

    FVector Direction = GetAimDirection();
    Direction.Z = Lift;
    Ball->ApplyKick(Direction, Strength, this);
    BallActionCooldownRemaining = BallActionCooldown;
    return true;
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

void ARFAPlayerCharacter::PlayBallActionSound(bool bStrongKick) const
{
    if (!GetWorld())
    {
        return;
    }

    for (TActorIterator<ARFAAudioManager> It(GetWorld()); It; ++It)
    {
        if (bStrongKick)
        {
            It->PlayKick();
        }
        else
        {
            It->PlayPass();
        }

        return;
    }
}

void ARFAPlayerCharacter::ApplyVisualColor(UStaticMeshComponent* MeshComponent, const FLinearColor& Color) const
{
    if (!MeshComponent)
    {
        return;
    }

    if (UMaterialInstanceDynamic* DynamicMaterial = MeshComponent->CreateAndSetMaterialInstanceDynamic(0))
    {
        DynamicMaterial->SetVectorParameterValue(TEXT("Color"), Color);
    }
}
