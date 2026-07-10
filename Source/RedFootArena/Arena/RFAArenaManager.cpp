#include "Arena/RFAArenaManager.h"
#include "Core/RFATypes.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Goals/RFAGoalActor.h"
#include "Materials/MaterialInterface.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "UObject/ConstructorHelpers.h"

ARFAArenaManager::ARFAArenaManager()
{
    PrimaryActorTick.bCanEverTick = false;

    SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
    RootComponent = SceneRoot;
    SceneRoot->SetMobility(EComponentMobility::Static);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeAsset(TEXT("/Engine/BasicShapes/Cube.Cube"));
    if (CubeAsset.Succeeded())
    {
        CubeMesh = CubeAsset.Object;
    }

    static ConstructorHelpers::FObjectFinder<UMaterialInterface> AsphaltAsset(TEXT("/Game/External/PolyHaven/M_Asphalt.M_Asphalt"));
    if (AsphaltAsset.Succeeded())
    {
        AsphaltMaterial = AsphaltAsset.Object;
    }

    static ConstructorHelpers::FObjectFinder<UMaterialInterface> BrickAsset(TEXT("/Game/External/PolyHaven/M_BrickWall.M_BrickWall"));
    if (BrickAsset.Succeeded())
    {
        BrickMaterial = BrickAsset.Object;
    }

    GoalActorClass = ARFAGoalActor::StaticClass();
}

void ARFAArenaManager::BuildArena()
{
    const float HalfLength = ArenaLength * 0.5f;
    const float HalfWidth = ArenaWidth * 0.5f;
    const float EndWallSegmentWidth = (ArenaWidth - GoalWidth) * 0.5f;
    const float EndWallY = GoalWidth * 0.5f + EndWallSegmentWidth * 0.5f;
    const FLinearColor AsphaltColor(0.12f, 0.13f, 0.15f, 1.0f);
    const FLinearColor WallColor(0.28f, 0.24f, 0.22f, 1.0f);
    const FLinearColor MarkingColor(0.92f, 0.96f, 0.9f, 1.0f);
    const FLinearColor HomeGoalColor(0.88f, 0.18f, 0.14f, 1.0f);
    const FLinearColor AwayGoalColor(0.12f, 0.42f, 0.95f, 1.0f);

    CreateArenaPiece(TEXT("Floor"), FVector(0.0f, 0.0f, -5.0f), FVector(ArenaLength + GoalDepth * 2.0f, ArenaWidth, 10.0f), true, AsphaltColor, AsphaltMaterial);

    CreateArenaPiece(TEXT("CenterLine"), FVector(0.0f, 0.0f, 3.0f), FVector(16.0f, ArenaWidth - WallThickness * 2.0f, 4.0f), false, MarkingColor);
    CreateArenaPiece(TEXT("CenterSpot"), FVector(0.0f, 0.0f, 4.0f), FVector(34.0f, 34.0f, 5.0f), false, MarkingColor);
    CreateArenaPiece(TEXT("LeftTouchLine"), FVector(0.0f, -HalfWidth + 48.0f, 3.0f), FVector(ArenaLength, 14.0f, 4.0f), false, MarkingColor);
    CreateArenaPiece(TEXT("RightTouchLine"), FVector(0.0f, HalfWidth - 48.0f, 3.0f), FVector(ArenaLength, 14.0f, 4.0f), false, MarkingColor);
    CreateArenaPiece(TEXT("HomeBoxLine"), FVector(-HalfLength + 430.0f, 0.0f, 3.0f), FVector(14.0f, GoalWidth + 360.0f, 4.0f), false, MarkingColor);
    CreateArenaPiece(TEXT("AwayBoxLine"), FVector(HalfLength - 430.0f, 0.0f, 3.0f), FVector(14.0f, GoalWidth + 360.0f, 4.0f), false, MarkingColor);
    CreateArenaPiece(TEXT("HomeGoalLine"), FVector(-HalfLength + 8.0f, 0.0f, 3.0f), FVector(16.0f, GoalWidth, 4.0f), false, HomeGoalColor);
    CreateArenaPiece(TEXT("AwayGoalLine"), FVector(HalfLength - 8.0f, 0.0f, 3.0f), FVector(16.0f, GoalWidth, 4.0f), false, AwayGoalColor);

    CreateArenaPiece(TEXT("LeftSideWall"), FVector(0.0f, -HalfWidth, WallHeight * 0.5f), FVector(ArenaLength + GoalDepth * 2.0f, WallThickness, WallHeight), true, WallColor, BrickMaterial);
    CreateArenaPiece(TEXT("RightSideWall"), FVector(0.0f, HalfWidth, WallHeight * 0.5f), FVector(ArenaLength + GoalDepth * 2.0f, WallThickness, WallHeight), true, WallColor, BrickMaterial);

    CreateArenaPiece(TEXT("HomeEndWallLeft"), FVector(-HalfLength, -EndWallY, WallHeight * 0.5f), FVector(WallThickness, EndWallSegmentWidth, WallHeight), true, WallColor, BrickMaterial);
    CreateArenaPiece(TEXT("HomeEndWallRight"), FVector(-HalfLength, EndWallY, WallHeight * 0.5f), FVector(WallThickness, EndWallSegmentWidth, WallHeight), true, WallColor, BrickMaterial);
    CreateArenaPiece(TEXT("AwayEndWallLeft"), FVector(HalfLength, -EndWallY, WallHeight * 0.5f), FVector(WallThickness, EndWallSegmentWidth, WallHeight), true, WallColor, BrickMaterial);
    CreateArenaPiece(TEXT("AwayEndWallRight"), FVector(HalfLength, EndWallY, WallHeight * 0.5f), FVector(WallThickness, EndWallSegmentWidth, WallHeight), true, WallColor, BrickMaterial);

    CreateArenaPiece(TEXT("HomeGoalBackWall"), FVector(-HalfLength - GoalDepth, 0.0f, WallHeight * 0.5f), FVector(WallThickness, GoalWidth, WallHeight), true, HomeGoalColor);
    CreateArenaPiece(TEXT("AwayGoalBackWall"), FVector(HalfLength + GoalDepth, 0.0f, WallHeight * 0.5f), FVector(WallThickness, GoalWidth, WallHeight), true, AwayGoalColor);

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

UStaticMeshComponent* ARFAArenaManager::CreateArenaPiece(const FString& Name, const FVector& Location, const FVector& Size, bool bBlocksMovement, const FLinearColor& Color, UMaterialInterface* Material)
{
    if (!CubeMesh)
    {
        return nullptr;
    }

    const FName ComponentName = MakeUniqueObjectName(this, UStaticMeshComponent::StaticClass(), FName(*Name));
    UStaticMeshComponent* Piece = NewObject<UStaticMeshComponent>(this, ComponentName);
    Piece->SetStaticMesh(CubeMesh);
    Piece->SetMobility(EComponentMobility::Static);
    Piece->SetCollisionEnabled(bBlocksMovement ? ECollisionEnabled::QueryAndPhysics : ECollisionEnabled::NoCollision);
    Piece->SetCollisionObjectType(ECC_WorldStatic);
    Piece->SetCollisionResponseToAllChannels(ECR_Block);
    Piece->AttachToComponent(SceneRoot, FAttachmentTransformRules::KeepRelativeTransform);
    Piece->SetRelativeLocation(Location);
    Piece->SetRelativeScale3D(Size / 100.0f);
    if (Material)
    {
        Piece->SetMaterial(0, Material);
    }
    else if (UMaterialInstanceDynamic* DynamicMaterial = Piece->CreateAndSetMaterialInstanceDynamic(0))
    {
        DynamicMaterial->SetVectorParameterValue(TEXT("Color"), Color);
    }
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
