#pragma once

#include "CoreMinimal.h"
#include "RFATypes.generated.h"

UENUM(BlueprintType)
enum class ERedFootTeam : uint8
{
    Home UMETA(DisplayName = "Home"),
    Away UMETA(DisplayName = "Away")
};

FORCEINLINE ERedFootTeam GetOppositeTeam(ERedFootTeam Team)
{
    return Team == ERedFootTeam::Home ? ERedFootTeam::Away : ERedFootTeam::Home;
}

