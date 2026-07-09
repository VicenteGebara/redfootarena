#include "Bot/RFABotCharacter.h"
#include "Bot/RFASimpleBotController.h"

ARFABotCharacter::ARFABotCharacter()
{
    AIControllerClass = ARFASimpleBotController::StaticClass();
    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

