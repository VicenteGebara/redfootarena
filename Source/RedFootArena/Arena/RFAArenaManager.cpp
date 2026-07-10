#include "Arena/RFAArenaManager.h"
#include "Core/RFATypes.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Goals/RFAGoalActor.h"
#include "Materials/MaterialInstanceDynamic.h"
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

    static ConstructorHelpers::FObjectFinder<UStaticMesh> BannerAsset(TEXT("/Game/External/Kenney/MiniArena/banner.banner"));
    if (BannerAsset.Succeeded())
    {
        BannerMesh = BannerAsset.Object;
    }

    static ConstructorHelpers::FObjectFinder<UStaticMesh> ColumnAsset(TEXT("/Game/External/Kenney/MiniArena/column.column"));
    if (ColumnAsset.Succeeded())
    {
        ColumnMesh = ColumnAsset.Object;
    }

    static ConstructorHelpers::FObjectFinder<UStaticMesh> TrophyAsset(TEXT("/Game/External/Kenney/MiniArena/trophy.trophy"));
    if (TrophyAsset.Succeeded())
    {
        TrophyMesh = TrophyAsset.Object;
    }

    static ConstructorHelpers::FObjectFinder<UStaticMesh> GateAsset(TEXT("/Game/External/Kenney/MiniArena/wall-gate.wall-gate"));
    if (GateAsset.Succeeded())
    {
        GateMesh = GateAsset.Object;
    }

    static ConstructorHelpers::FObjectFinder<UStaticMesh> RedFlagAsset(TEXT("/Game/External/Kenney/MinigolfKit/flag-large-red.flag-large-red"));
    if (RedFlagAsset.Succeeded())
    {
        RedFlagMesh = RedFlagAsset.Object;
    }

    static ConstructorHelpers::FObjectFinder<UStaticMesh> BlueFlagAsset(TEXT("/Game/External/Kenney/MinigolfKit/flag-large-blue.flag-large-blue"));
    if (BlueFlagAsset.Succeeded())
    {
        BlueFlagMesh = BlueFlagAsset.Object;
    }

    GoalActorClass = ARFAGoalActor::StaticClass();
}

void ARFAArenaManager::BuildArena()
{
    const float HalfLength = ArenaLength * 0.5f;
    const float HalfWidth = ArenaWidth * 0.5f;
    const float EndWallSegmentWidth = (ArenaWidth - GoalWidth) * 0.5f;
    const float EndWallY = GoalWidth * 0.5f + EndWallSegmentWidth * 0.5f;
    const FLinearColor TurfColor(0.08f, 0.36f, 0.19f, 1.0f);
    const FLinearColor WallColor(0.06f, 0.07f, 0.08f, 1.0f);
    const FLinearColor MarkingColor(0.92f, 0.96f, 0.9f, 1.0f);
    const FLinearColor HomeGoalColor(0.88f, 0.18f, 0.14f, 1.0f);
    const FLinearColor AwayGoalColor(0.12f, 0.42f, 0.95f, 1.0f);

    CreateArenaPiece(TEXT("Floor"), FVector(0.0f, 0.0f, -5.0f), FVector(ArenaLength + GoalDepth * 2.0f, ArenaWidth, 10.0f), true, TurfColor);

    CreateArenaPiece(TEXT("CenterLine"), FVector(0.0f, 0.0f, 3.0f), FVector(16.0f, ArenaWidth - WallThickness * 2.0f, 4.0f), false, MarkingColor);
    CreateArenaPiece(TEXT("CenterSpot"), FVector(0.0f, 0.0f, 5.0f), FVector(90.0f, 90.0f, 6.0f), false, MarkingColor);
    CreateArenaPiece(TEXT("HomeGoalLine"), FVector(-HalfLength + 8.0f, 0.0f, 3.0f), FVector(16.0f, GoalWidth, 4.0f), false, HomeGoalColor);
    CreateArenaPiece(TEXT("AwayGoalLine"), FVector(HalfLength - 8.0f, 0.0f, 3.0f), FVector(16.0f, GoalWidth, 4.0f), false, AwayGoalColor);

    CreateArenaPiece(TEXT("LeftSideWall"), FVector(0.0f, -HalfWidth, WallHeight * 0.5f), FVector(ArenaLength + GoalDepth * 2.0f, WallThickness, WallHeight), true, WallColor);
    CreateArenaPiece(TEXT("RightSideWall"), FVector(0.0f, HalfWidth, WallHeight * 0.5f), FVector(ArenaLength + GoalDepth * 2.0f, WallThickness, WallHeight), true, WallColor);

    CreateArenaPiece(TEXT("HomeEndWallLeft"), FVector(-HalfLength, -EndWallY, WallHeight * 0.5f), FVector(WallThickness, EndWallSegmentWidth, WallHeight), true, WallColor);
    CreateArenaPiece(TEXT("HomeEndWallRight"), FVector(-HalfLength, EndWallY, WallHeight * 0.5f), FVector(WallThickness, EndWallSegmentWidth, WallHeight), true, WallColor);
    CreateArenaPiece(TEXT("AwayEndWallLeft"), FVector(HalfLength, -EndWallY, WallHeight * 0.5f), FVector(WallThickness, EndWallSegmentWidth, WallHeight), true, WallColor);
    CreateArenaPiece(TEXT("AwayEndWallRight"), FVector(HalfLength, EndWallY, WallHeight * 0.5f), FVector(WallThickness, EndWallSegmentWidth, WallHeight), true, WallColor);

    CreateArenaPiece(TEXT("HomeGoalBackWall"), FVector(-HalfLength - GoalDepth, 0.0f, WallHeight * 0.5f), FVector(WallThickness, GoalWidth, WallHeight), true, HomeGoalColor);
    CreateArenaPiece(TEXT("AwayGoalBackWall"), FVector(HalfLength + GoalDepth, 0.0f, WallHeight * 0.5f), FVector(WallThickness, GoalWidth, WallHeight), true, AwayGoalColor);

    BuildDecorativeProps(HalfLength, HalfWidth);

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

UStaticMeshComponent* ARFAArenaManager::CreateArenaPiece(const FString& Name, const FVector& Location, const FVector& Size, bool bBlocksMovement, const FLinearColor& Color)
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
    if (UMaterialInstanceDynamic* DynamicMaterial = Piece->CreateAndSetMaterialInstanceDynamic(0))
    {
        DynamicMaterial->SetVectorParameterValue(TEXT("Color"), Color);
    }
    AddInstanceComponent(Piece);
    Piece->RegisterComponent();

    ArenaPieces.Add(Piece);
    return Piece;
}

UStaticMeshComponent* ARFAArenaManager::CreateDecorativeProp(const FString& Name, UStaticMesh* Mesh, const FVector& Location, const FRotator& Rotation, const FVector& Scale)
{
    if (!Mesh)
    {
        return nullptr;
    }

    const FName ComponentName = MakeUniqueObjectName(this, UStaticMeshComponent::StaticClass(), FName(*Name));
    UStaticMeshComponent* Prop = NewObject<UStaticMeshComponent>(this, ComponentName);
    Prop->SetStaticMesh(Mesh);
    Prop->SetMobility(EComponentMobility::Static);
    Prop->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    Prop->AttachToComponent(SceneRoot, FAttachmentTransformRules::KeepRelativeTransform);
    Prop->SetRelativeLocation(Location);
    Prop->SetRelativeRotation(Rotation);
    Prop->SetRelativeScale3D(Scale);
    AddInstanceComponent(Prop);
    Prop->RegisterComponent();

    ArenaPieces.Add(Prop);
    return Prop;
}

void ARFAArenaManager::BuildDecorativeProps(float HalfLength, float HalfWidth)
{
    const float CornerX = HalfLength - 170.0f;
    const float CornerY = HalfWidth - 120.0f;
    const float FlagY = GoalWidth * 0.5f + 95.0f;

    CreateDecorativeProp(TEXT("HomeLeftColumn"), ColumnMesh, FVector(-CornerX, -CornerY, 0.0f), FRotator::ZeroRotator, FVector(1.35f));
    CreateDecorativeProp(TEXT("HomeRightColumn"), ColumnMesh, FVector(-CornerX, CornerY, 0.0f), FRotator::ZeroRotator, FVector(1.35f));
    CreateDecorativeProp(TEXT("AwayLeftColumn"), ColumnMesh, FVector(CornerX, -CornerY, 0.0f), FRotator::ZeroRotator, FVector(1.35f));
    CreateDecorativeProp(TEXT("AwayRightColumn"), ColumnMesh, FVector(CornerX, CornerY, 0.0f), FRotator::ZeroRotator, FVector(1.35f));

    CreateDecorativeProp(TEXT("LeftBanner"), BannerMesh, FVector(0.0f, -HalfWidth + 12.0f, WallHeight * 0.62f), FRotator(0.0f, 90.0f, 0.0f), FVector(1.6f));
    CreateDecorativeProp(TEXT("RightBanner"), BannerMesh, FVector(0.0f, HalfWidth - 12.0f, WallHeight * 0.62f), FRotator(0.0f, -90.0f, 0.0f), FVector(1.6f));

    CreateDecorativeProp(TEXT("HomeGate"), GateMesh, FVector(-HalfLength - GoalDepth - 38.0f, 0.0f, 0.0f), FRotator(0.0f, 90.0f, 0.0f), FVector(1.35f));
    CreateDecorativeProp(TEXT("AwayGate"), GateMesh, FVector(HalfLength + GoalDepth + 38.0f, 0.0f, 0.0f), FRotator(0.0f, -90.0f, 0.0f), FVector(1.35f));

    CreateDecorativeProp(TEXT("HomeTrophy"), TrophyMesh, FVector(-HalfLength - GoalDepth - 115.0f, 0.0f, 18.0f), FRotator(0.0f, 90.0f, 0.0f), FVector(1.15f));
    CreateDecorativeProp(TEXT("AwayTrophy"), TrophyMesh, FVector(HalfLength + GoalDepth + 115.0f, 0.0f, 18.0f), FRotator(0.0f, -90.0f, 0.0f), FVector(1.15f));

    CreateDecorativeProp(TEXT("HomeFlagLeft"), RedFlagMesh, FVector(-HalfLength - 35.0f, -FlagY, 0.0f), FRotator(0.0f, 90.0f, 0.0f), FVector(1.15f));
    CreateDecorativeProp(TEXT("HomeFlagRight"), RedFlagMesh, FVector(-HalfLength - 35.0f, FlagY, 0.0f), FRotator(0.0f, 90.0f, 0.0f), FVector(1.15f));
    CreateDecorativeProp(TEXT("AwayFlagLeft"), BlueFlagMesh, FVector(HalfLength + 35.0f, -FlagY, 0.0f), FRotator(0.0f, -90.0f, 0.0f), FVector(1.15f));
    CreateDecorativeProp(TEXT("AwayFlagRight"), BlueFlagMesh, FVector(HalfLength + 35.0f, FlagY, 0.0f), FRotator(0.0f, -90.0f, 0.0f), FVector(1.15f));
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
