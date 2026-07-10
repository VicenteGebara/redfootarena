#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RFAGameMode.generated.h"

class ARFAArenaManager;
class ARFAAudioManager;
class ARFABallActor;
class ARFABotCharacter;
class ARFAMatchManager;
class ARFAPlayerCharacter;
class ARFAScoreManager;

UCLASS()
class REDFOOTARENA_API ARFAGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    ARFAGameMode();

    virtual void StartPlay() override;

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Classes")
    TSubclassOf<ARFAArenaManager> ArenaManagerClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Classes")
    TSubclassOf<ARFAAudioManager> AudioManagerClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Classes")
    TSubclassOf<ARFAMatchManager> MatchManagerClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Classes")
    TSubclassOf<ARFAScoreManager> ScoreManagerClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Classes")
    TSubclassOf<ARFABallActor> BallClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Classes")
    TSubclassOf<ARFAPlayerCharacter> PlayerCharacterClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Classes")
    TSubclassOf<ARFABotCharacter> BotCharacterClass;

private:
    void InitializeOfflineMatch();
    void BuildVisualEnvironment(UWorld* World) const;
    APawn* EnsurePlayerPawn(const FTransform& SpawnTransform);
    APawn* SpawnBotPawn(const FTransform& SpawnTransform);

    UPROPERTY()
    ARFAArenaManager* ArenaManager;

    UPROPERTY()
    ARFAAudioManager* AudioManager;

    UPROPERTY()
    ARFAMatchManager* MatchManager;

    UPROPERTY()
    ARFAScoreManager* ScoreManager;

    UPROPERTY()
    ARFABallActor* Ball;
};
