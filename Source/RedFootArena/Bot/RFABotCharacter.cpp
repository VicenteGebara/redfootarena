#include "Bot/RFABotCharacter.h"
#include "Bot/RFASimpleBotController.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/SkeletalMesh.h"
#include "UObject/ConstructorHelpers.h"

ARFABotCharacter::ARFABotCharacter()
{
    BodyColor = FLinearColor(0.1f, 0.35f, 1.0f, 1.0f);
    AccentColor = FLinearColor(0.36f, 0.92f, 1.0f, 1.0f);

    static ConstructorHelpers::FObjectFinder<USkeletalMesh> AwayHumanAsset(
        TEXT("/Game/External/Kenney/MiniCharacters/character-male-b.character-male-b"));
    if (AwayHumanAsset.Succeeded())
    {
        GetMesh()->SetSkeletalMesh(AwayHumanAsset.Object);
    }

    AIControllerClass = ARFASimpleBotController::StaticClass();
    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}
