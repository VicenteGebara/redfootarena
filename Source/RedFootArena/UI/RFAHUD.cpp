#include "UI/RFAHUD.h"
#include "EngineUtils.h"
#include "Match/RFAMatchManager.h"
#include "Match/RFAScoreManager.h"

void ARFAHUD::DrawHUD()
{
    Super::DrawHUD();

    ARFAMatchManager* MatchManager = FindMatchManager();
    if (!MatchManager || !MatchManager->GetScoreManager())
    {
        return;
    }

    const ARFAScoreManager* ScoreManager = MatchManager->GetScoreManager();
    const int32 RemainingSeconds = FMath::CeilToInt(MatchManager->GetRemainingTime());
    const int32 Minutes = RemainingSeconds / 60;
    const int32 Seconds = RemainingSeconds % 60;

    const FString ScoreText = FString::Printf(TEXT("Home %d  x  %d Away"), ScoreManager->GetHomeScore(), ScoreManager->GetAwayScore());
    const FString TimeText = FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds);

    DrawText(ScoreText, FLinearColor::White, 32.0f, 28.0f, nullptr, 1.8f, false);
    DrawText(TimeText, FLinearColor::White, 32.0f, 70.0f, nullptr, 1.5f, false);
}

ARFAMatchManager* ARFAHUD::FindMatchManager() const
{
    if (!GetWorld())
    {
        return nullptr;
    }

    for (TActorIterator<ARFAMatchManager> It(GetWorld()); It; ++It)
    {
        return *It;
    }

    return nullptr;
}

