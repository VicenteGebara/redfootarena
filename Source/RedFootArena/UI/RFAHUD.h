#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "RFAHUD.generated.h"

class ARFAMatchManager;
class URFAStaminaComponent;

UCLASS()
class REDFOOTARENA_API ARFAHUD : public AHUD
{
    GENERATED_BODY()

public:
    virtual void DrawHUD() override;

private:
    ARFAMatchManager* FindMatchManager() const;
    URFAStaminaComponent* FindPlayerStamina() const;
    void DrawScoreboard(const ARFAMatchManager& MatchManager);
    void DrawMatchStatus(const ARFAMatchManager& MatchManager);
    void DrawStaminaBar(const URFAStaminaComponent& StaminaComponent);
};
