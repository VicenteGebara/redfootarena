#include "Ball/RFABallActor.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"

ARFABallActor::ARFABallActor()
{
    PrimaryActorTick.bCanEverTick = false;

    BallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BallMesh"));
    RootComponent = BallMesh;

    static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMesh(TEXT("/Engine/BasicShapes/Sphere.Sphere"));
    if (SphereMesh.Succeeded())
    {
        BallMesh->SetStaticMesh(SphereMesh.Object);
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

