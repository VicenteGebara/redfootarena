#include "Goals/RFAGoalActor.h"
#include "Ball/RFABallActor.h"
#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "UObject/ConstructorHelpers.h"

ARFAGoalActor::ARFAGoalActor()
{
    PrimaryActorTick.bCanEverTick = false;

    SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
    RootComponent = SceneRoot;

    GoalTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("GoalTrigger"));
    GoalTrigger->SetupAttachment(SceneRoot);
    GoalTrigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    GoalTrigger->SetCollisionObjectType(ECC_WorldDynamic);
    GoalTrigger->SetCollisionResponseToAllChannels(ECR_Ignore);
    GoalTrigger->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Overlap);
    GoalTrigger->SetGenerateOverlapEvents(true);
    GoalTrigger->OnComponentBeginOverlap.AddDynamic(this, &ARFAGoalActor::HandleGoalOverlap);

    LeftPost = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftPost"));
    LeftPost->SetupAttachment(SceneRoot);
    LeftPost->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

    RightPost = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightPost"));
    RightPost->SetupAttachment(SceneRoot);
    RightPost->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

    Crossbar = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Crossbar"));
    Crossbar->SetupAttachment(SceneRoot);
    Crossbar->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> CylinderMesh(TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
    if (CylinderMesh.Succeeded())
    {
        LeftPost->SetStaticMesh(CylinderMesh.Object);
        RightPost->SetStaticMesh(CylinderMesh.Object);
        Crossbar->SetStaticMesh(CylinderMesh.Object);
    }
}

void ARFAGoalActor::ConfigureGoal(ERedFootTeam InDefendingTeam, float InGoalWidth, float InGoalHeight, float InGoalDepth)
{
    DefendingTeam = InDefendingTeam;

    const float PostThickness = 18.0f;
    const float MouthX = DefendingTeam == ERedFootTeam::Home
        ? InGoalDepth * 0.5f
        : -InGoalDepth * 0.5f;
    GoalTrigger->SetBoxExtent(FVector(InGoalDepth * 0.5f, InGoalWidth * 0.5f, InGoalHeight * 0.5f));
    GoalTrigger->SetRelativeLocation(FVector(0.0f, 0.0f, InGoalHeight * 0.5f));

    LeftPost->SetRelativeLocation(FVector(MouthX, -InGoalWidth * 0.5f, InGoalHeight * 0.5f));
    LeftPost->SetRelativeScale3D(FVector(PostThickness / 100.0f, PostThickness / 100.0f, InGoalHeight / 100.0f));

    RightPost->SetRelativeLocation(FVector(MouthX, InGoalWidth * 0.5f, InGoalHeight * 0.5f));
    RightPost->SetRelativeScale3D(FVector(PostThickness / 100.0f, PostThickness / 100.0f, InGoalHeight / 100.0f));

    Crossbar->SetRelativeLocation(FVector(MouthX, 0.0f, InGoalHeight));
    Crossbar->SetRelativeRotation(FRotator(0.0f, 0.0f, 90.0f));
    Crossbar->SetRelativeScale3D(FVector(PostThickness / 100.0f, PostThickness / 100.0f, (InGoalWidth + PostThickness) / 100.0f));

    const FLinearColor GoalColor = DefendingTeam == ERedFootTeam::Home
        ? FLinearColor(0.95f, 0.12f, 0.08f, 1.0f)
        : FLinearColor(0.08f, 0.38f, 1.0f, 1.0f);

    for (UStaticMeshComponent* GoalPiece : { LeftPost, RightPost, Crossbar })
    {
        if (GoalPiece)
        {
            if (UMaterialInstanceDynamic* DynamicMaterial = GoalPiece->CreateAndSetMaterialInstanceDynamic(0))
            {
                DynamicMaterial->SetVectorParameterValue(TEXT("Color"), GoalColor);
            }
        }
    }
}

void ARFAGoalActor::HandleGoalOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    ARFABallActor* Ball = Cast<ARFABallActor>(OtherActor);
    if (!Ball)
    {
        return;
    }

    OnGoalScored.Broadcast(GetOppositeTeam(DefendingTeam), this, Ball);
}
