#include "Bot/RFABotCharacter.h"
#include "Bot/RFASimpleBotController.h"

ARFABotCharacter::ARFABotCharacter()
{
    BodyColor = FLinearColor(0.1f, 0.35f, 1.0f, 1.0f);
    AccentColor = FLinearColor(0.36f, 0.92f, 1.0f, 1.0f);
    AIControllerClass = ARFASimpleBotController::StaticClass();
    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}
