#include "Goals/RFAGoalActor.h"
#include "Ball/RFABallActor.h"
#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
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
    LeftPost->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    RightPost = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightPost"));
    RightPost->SetupAttachment(SceneRoot);
    RightPost->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    Crossbar = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Crossbar"));
    Crossbar->SetupAttachment(SceneRoot);
    Crossbar->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("/Engine/BasicShapes/Cube.Cube"));
    if (CubeMesh.Succeeded())
    {
        LeftPost->SetStaticMesh(CubeMesh.Object);
        RightPost->SetStaticMesh(CubeMesh.Object);
        Crossbar->SetStaticMesh(CubeMesh.Object);
    }
}

void ARFAGoalActor::ConfigureGoal(ERedFootTeam InDefendingTeam, float InGoalWidth, float InGoalHeight, float InGoalDepth)
{
    DefendingTeam = InDefendingTeam;

    const float PostThickness = 18.0f;
    GoalTrigger->SetBoxExtent(FVector(InGoalDepth * 0.5f, InGoalWidth * 0.5f, InGoalHeight * 0.5f));
    GoalTrigger->SetRelativeLocation(FVector(0.0f, 0.0f, InGoalHeight * 0.5f));

    LeftPost->SetRelativeLocation(FVector(0.0f, -InGoalWidth * 0.5f, InGoalHeight * 0.5f));
    LeftPost->SetRelativeScale3D(FVector(InGoalDepth / 100.0f, PostThickness / 100.0f, InGoalHeight / 100.0f));

    RightPost->SetRelativeLocation(FVector(0.0f, InGoalWidth * 0.5f, InGoalHeight * 0.5f));
    RightPost->SetRelativeScale3D(FVector(InGoalDepth / 100.0f, PostThickness / 100.0f, InGoalHeight / 100.0f));

    Crossbar->SetRelativeLocation(FVector(0.0f, 0.0f, InGoalHeight));
    Crossbar->SetRelativeScale3D(FVector(InGoalDepth / 100.0f, (InGoalWidth + PostThickness) / 100.0f, PostThickness / 100.0f));
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
