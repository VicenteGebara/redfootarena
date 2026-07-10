#include "Game/RFAGameMode.h"
#include "Arena/RFAArenaManager.h"
#include "Ball/RFABallActor.h"
#include "Bot/RFABotCharacter.h"
#include "Goals/RFAGoalActor.h"
#include "Match/RFAMatchManager.h"
#include "Match/RFAScoreManager.h"
#include "Player/RFAPlayerCharacter.h"
#include "UI/RFAHUD.h"
#include "Components/DirectionalLightComponent.h"
#include "Components/SkyLightComponent.h"
#include "Engine/DirectionalLight.h"
#include "Engine/SkyLight.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"

ARFAGameMode::ARFAGameMode()
{
    ArenaManagerClass = ARFAArenaManager::StaticClass();
    MatchManagerClass = ARFAMatchManager::StaticClass();
    ScoreManagerClass = ARFAScoreManager::StaticClass();
    BallClass = ARFABallActor::StaticClass();
    PlayerCharacterClass = ARFAPlayerCharacter::StaticClass();
    BotCharacterClass = ARFABotCharacter::StaticClass();

    DefaultPawnClass = PlayerCharacterClass;
    HUDClass = ARFAHUD::StaticClass();
}

void ARFAGameMode::StartPlay()
{
    Super::StartPlay();
    InitializeOfflineMatch();
}

void ARFAGameMode::InitializeOfflineMatch()
{
    UWorld* World = GetWorld();
    if (!World)
    {
        return;
    }

    BuildVisualEnvironment(World);

    ArenaManager = World->SpawnActor<ARFAArenaManager>(ArenaManagerClass, FVector::ZeroVector, FRotator::ZeroRotator);
    if (!ArenaManager)
    {
        return;
    }

    ArenaManager->BuildArena();

    ScoreManager = World->SpawnActor<ARFAScoreManager>(ScoreManagerClass);
    MatchManager = World->SpawnActor<ARFAMatchManager>(MatchManagerClass);
    Ball = World->SpawnActor<ARFABallActor>(BallClass, ArenaManager->GetBallSpawnTransform());

    if (!MatchManager || !ScoreManager || !Ball)
    {
        return;
    }

    MatchManager->InitializeMatch(ScoreManager, Ball, ArenaManager->GetBallSpawnTransform().GetLocation());

    for (ARFAGoalActor* Goal : ArenaManager->GetGoals())
    {
        MatchManager->RegisterGoal(Goal);
    }

    APawn* PlayerPawn = EnsurePlayerPawn(ArenaManager->GetHomePlayerSpawnTransform());
    APawn* BotPawn = SpawnBotPawn(ArenaManager->GetAwayPlayerSpawnTransform());

    MatchManager->RegisterResetActor(PlayerPawn, ArenaManager->GetHomePlayerSpawnTransform());
    MatchManager->RegisterResetActor(BotPawn, ArenaManager->GetAwayPlayerSpawnTransform());
}

void ARFAGameMode::BuildVisualEnvironment(UWorld* World) const
{
    if (!World)
    {
        return;
    }

    ADirectionalLight* KeyLight = World->SpawnActor<ADirectionalLight>(FVector(-900.0f, -650.0f, 1400.0f), FRotator(-42.0f, -28.0f, 0.0f));
    if (KeyLight && KeyLight->GetLightComponent())
    {
        KeyLight->GetLightComponent()->SetIntensity(3.2f);
        KeyLight->GetLightComponent()->SetLightColor(FLinearColor(1.0f, 0.94f, 0.82f, 1.0f));
    }

    ASkyLight* FillLight = World->SpawnActor<ASkyLight>(FVector::ZeroVector, FRotator::ZeroRotator);
    if (FillLight && FillLight->GetLightComponent())
    {
        FillLight->GetLightComponent()->SetIntensity(0.65f);
        FillLight->GetLightComponent()->SetLightColor(FLinearColor(0.72f, 0.82f, 1.0f, 1.0f));
    }
}

APawn* ARFAGameMode::EnsurePlayerPawn(const FTransform& SpawnTransform)
{
    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
    if (!PlayerController || !GetWorld())
    {
        return nullptr;
    }

    APawn* ExistingPawn = PlayerController->GetPawn();
    if (ExistingPawn)
    {
        ExistingPawn->SetActorTransform(SpawnTransform, false, nullptr, ETeleportType::TeleportPhysics);
        if (ARFAPlayerCharacter* PlayerCharacter = Cast<ARFAPlayerCharacter>(ExistingPawn))
        {
            PlayerCharacter->ResetArcadeVelocity();
        }
        return ExistingPawn;
    }

    APawn* SpawnedPawn = GetWorld()->SpawnActor<ARFAPlayerCharacter>(PlayerCharacterClass, SpawnTransform);
    if (SpawnedPawn)
    {
        PlayerController->Possess(SpawnedPawn);
    }

    return SpawnedPawn;
}

APawn* ARFAGameMode::SpawnBotPawn(const FTransform& SpawnTransform)
{
    if (!GetWorld() || !BotCharacterClass)
    {
        return nullptr;
    }

    APawn* BotPawn = GetWorld()->SpawnActor<ARFABotCharacter>(BotCharacterClass, SpawnTransform);
    if (BotPawn)
    {
        BotPawn->SpawnDefaultController();
    }

    return BotPawn;
}
