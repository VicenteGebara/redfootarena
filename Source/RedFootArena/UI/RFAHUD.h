#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "RFAHUD.generated.h"

class ARFAMatchManager;

UCLASS()
class REDFOOTARENA_API ARFAHUD : public AHUD
{
    GENERATED_BODY()

public:
    virtual void DrawHUD() override;

private:
    ARFAMatchManager* FindMatchManager() const;
};

