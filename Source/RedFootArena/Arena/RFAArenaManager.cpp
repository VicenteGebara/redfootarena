#include "Arena/RFAArenaManager.h"
#include "Core/RFATypes.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Goals/RFAGoalActor.h"
#include "UObject/ConstructorHelpers.h"

ARFAArenaManager::ARFAArenaManager()
{
    PrimaryActorTick.bCanEverTick = false;

    SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
    RootComponent = SceneRoot;

    static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeAsset(TEXT("/Engine/BasicShapes/Cube.Cube"));
    if (CubeAsset.Succeeded())
    {
        CubeMesh = CubeAsset.Object;
    }

    GoalActorClass = ARFAGoalActor::StaticClass();
}

void ARFAArenaManager::BuildArena()
{
    const float HalfLength = ArenaLength * 0.5f;
    const float HalfWidth = ArenaWidth * 0.5f;
    const float EndWallSegmentWidth = (ArenaWidth - GoalWidth) * 0.5f;
    const float EndWallY = GoalWidth * 0.5f + EndWallSegmentWidth * 0.5f;

    CreateArenaPiece(TEXT("Floor"), FVector(0.0f, 0.0f, -5.0f), FVector(ArenaLength + GoalDepth * 2.0f, ArenaWidth, 10.0f));

    CreateArenaPiece(TEXT("LeftSideWall"), FVector(0.0f, -HalfWidth, WallHeight * 0.5f), FVector(ArenaLength + GoalDepth * 2.0f, WallThickness, WallHeight));
    CreateArenaPiece(TEXT("RightSideWall"), FVector(0.0f, HalfWidth, WallHeight * 0.5f), FVector(ArenaLength + GoalDepth * 2.0f, WallThickness, WallHeight));

    CreateArenaPiece(TEXT("HomeEndWallLeft"), FVector(-HalfLength, -EndWallY, WallHeight * 0.5f), FVector(WallThickness, EndWallSegmentWidth, WallHeight));
    CreateArenaPiece(TEXT("HomeEndWallRight"), FVector(-HalfLength, EndWallY, WallHeight * 0.5f), FVector(WallThickness, EndWallSegmentWidth, WallHeight));
    CreateArenaPiece(TEXT("AwayEndWallLeft"), FVector(HalfLength, -EndWallY, WallHeight * 0.5f), FVector(WallThickness, EndWallSegmentWidth, WallHeight));
    CreateArenaPiece(TEXT("AwayEndWallRight"), FVector(HalfLength, EndWallY, WallHeight * 0.5f), FVector(WallThickness, EndWallSegmentWidth, WallHeight));

    CreateArenaPiece(TEXT("HomeGoalBackWall"), FVector(-HalfLength - GoalDepth, 0.0f, WallHeight * 0.5f), FVector(WallThickness, GoalWidth, WallHeight));
    CreateArenaPiece(TEXT("AwayGoalBackWall"), FVector(HalfLength + GoalDepth, 0.0f, WallHeight * 0.5f), FVector(WallThickness, GoalWidth, WallHeight));

    SpawnedGoals.Reset();
    SpawnedGoals.Add(SpawnGoal(-1.0f));
    SpawnedGoals.Add(SpawnGoal(1.0f));
}

FTransform ARFAArenaManager::GetHomePlayerSpawnTransform() const
{
    return FTransform(FRotator(0.0f, 0.0f, 0.0f), FVector(-ArenaLength * 0.25f, 0.0f, 110.0f));
}

FTransform ARFAArenaManager::GetAwayPlayerSpawnTransform() const
{
    return FTransform(FRotator(0.0f, 180.0f, 0.0f), FVector(ArenaLength * 0.25f, 0.0f, 110.0f));
}

FTransform ARFAArenaManager::GetBallSpawnTransform() const
{
    return FTransform(FRotator::ZeroRotator, FVector(0.0f, 0.0f, 60.0f));
}

UStaticMeshComponent* ARFAArenaManager::CreateArenaPiece(const FString& Name, const FVector& Location, const FVector& Size)
{
    if (!CubeMesh)
    {
        return nullptr;
    }

    const FName ComponentName = MakeUniqueObjectName(this, UStaticMeshComponent::StaticClass(), FName(*Name));
    UStaticMeshComponent* Piece = NewObject<UStaticMeshComponent>(this, ComponentName);
    Piece->SetStaticMesh(CubeMesh);
    Piece->SetMobility(EComponentMobility::Static);
    Piece->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    Piece->SetCollisionObjectType(ECC_WorldStatic);
    Piece->SetCollisionResponseToAllChannels(ECR_Block);
    Piece->AttachToComponent(SceneRoot, FAttachmentTransformRules::KeepRelativeTransform);
    Piece->SetRelativeLocation(Location);
    Piece->SetRelativeScale3D(Size / 100.0f);
    AddInstanceComponent(Piece);
    Piece->RegisterComponent();

    ArenaPieces.Add(Piece);
    return Piece;
}

ARFAGoalActor* ARFAArenaManager::SpawnGoal(float XSign)
{
    if (!GetWorld() || !GoalActorClass)
    {
        return nullptr;
    }

    const float HalfLength = ArenaLength * 0.5f;
    const FVector Location(XSign * (HalfLength + GoalDepth * 0.5f), 0.0f, 0.0f);
    ARFAGoalActor* Goal = GetWorld()->SpawnActor<ARFAGoalActor>(GoalActorClass, Location, FRotator::ZeroRotator);
    if (!Goal)
    {
        return nullptr;
    }

    const ERedFootTeam DefendingTeam = XSign < 0.0f ? ERedFootTeam::Home : ERedFootTeam::Away;
    Goal->ConfigureGoal(DefendingTeam, GoalWidth, GoalHeight, GoalDepth);
    return Goal;
}
