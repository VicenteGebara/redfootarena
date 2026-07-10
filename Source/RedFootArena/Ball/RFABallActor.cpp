#include "Ball/RFABallActor.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "UObject/ConstructorHelpers.h"

ARFABallActor::ARFABallActor()
{
    PrimaryActorTick.bCanEverTick = false;

    BallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BallMesh"));
    RootComponent = BallMesh;

    StripeMeshX = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StripeMeshX"));
    StripeMeshX->SetupAttachment(BallMesh);
    StripeMeshX->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    StripeMeshX->SetRelativeScale3D(FVector(0.52f, 0.08f, 0.08f));

    StripeMeshY = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StripeMeshY"));
    StripeMeshY->SetupAttachment(BallMesh);
    StripeMeshY->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    StripeMeshY->SetRelativeScale3D(FVector(0.08f, 0.52f, 0.08f));

    static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMesh(TEXT("/Engine/BasicShapes/Sphere.Sphere"));
    if (SphereMesh.Succeeded())
    {
        BallMesh->SetStaticMesh(SphereMesh.Object);
    }

    static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("/Engine/BasicShapes/Cube.Cube"));
    if (CubeMesh.Succeeded())
    {
        StripeMeshX->SetStaticMesh(CubeMesh.Object);
        StripeMeshY->SetStaticMesh(CubeMesh.Object);
    }

    BallMesh->SetRelativeScale3D(FVector(0.45f));
    BallMesh->SetSimulatePhysics(true);
    BallMesh->SetEnableGravity(true);
    BallMesh->SetLinearDamping(0.45f);
    BallMesh->SetAngularDamping(0.35f);
    BallMesh->SetMassOverrideInKg(NAME_None, 0.45f, true);
    BallMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    BallMesh->SetCollisionObjectType(ECC_PhysicsBody);
    BallMesh->SetCollisionResponseToAllChannels(ECR_Block);
    BallMesh->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
    BallMesh->SetGenerateOverlapEvents(true);
}

void ARFABallActor::BeginPlay()
{
    Super::BeginPlay();

    ApplyVisualColor(BallMesh, FLinearColor(0.96f, 0.96f, 0.9f, 1.0f));
    ApplyVisualColor(StripeMeshX, FLinearColor(0.9f, 0.06f, 0.04f, 1.0f));
    ApplyVisualColor(StripeMeshY, FLinearColor(0.05f, 0.08f, 0.1f, 1.0f));
}

void ARFABallActor::ApplyKick(const FVector& Direction, float ImpulseStrength, AActor* Kicker)
{
    if (!BallMesh)
    {
        return;
    }

    FVector SafeDirection = Direction.GetSafeNormal();
    if (SafeDirection.IsNearlyZero())
    {
        SafeDirection = GetActorForwardVector();
    }

    BallMesh->WakeAllRigidBodies();
    BallMesh->AddImpulse(SafeDirection * ImpulseStrength, NAME_None, true);

    const FVector CurrentVelocity = BallMesh->GetPhysicsLinearVelocity();
    if (CurrentVelocity.SizeSquared() > FMath::Square(MaxSpeed))
    {
        BallMesh->SetPhysicsLinearVelocity(CurrentVelocity.GetSafeNormal() * MaxSpeed);
    }
}

void ARFABallActor::ResetBall(const FVector& NewLocation)
{
    SetActorLocation(NewLocation, false, nullptr, ETeleportType::TeleportPhysics);
    SetActorRotation(FRotator::ZeroRotator);
    StopBall();
}

void ARFABallActor::StopBall()
{
    if (!BallMesh)
    {
        return;
    }

    BallMesh->SetPhysicsLinearVelocity(FVector::ZeroVector);
    BallMesh->SetPhysicsAngularVelocityInDegrees(FVector::ZeroVector);
    BallMesh->WakeAllRigidBodies();
}

void ARFABallActor::ApplyVisualColor(UStaticMeshComponent* MeshComponent, const FLinearColor& Color) const
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
