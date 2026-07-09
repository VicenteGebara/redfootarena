#include "UI/RFAHUD.h"
#include "Components/RFAStaminaComponent.h"
#include "Engine/Canvas.h"
#include "EngineUtils.h"
#include "GameFramework/PlayerController.h"
#include "Match/RFAMatchManager.h"
#include "Match/RFAScoreManager.h"
#include "Player/RFAPlayerCharacter.h"

void ARFAHUD::DrawHUD()
{
    Super::DrawHUD();

    ARFAMatchManager* MatchManager = FindMatchManager();
    if (!MatchManager || !MatchManager->GetScoreManager())
    {
        return;
    }

    DrawScoreboard(*MatchManager);
    DrawMatchStatus(*MatchManager);

    if (URFAStaminaComponent* StaminaComponent = FindPlayerStamina())
    {
        DrawStaminaBar(*StaminaComponent);
    }
}

void ARFAHUD::DrawScoreboard(const ARFAMatchManager& MatchManager)
{
    if (!Canvas || !MatchManager.GetScoreManager())
    {
        return;
    }

    const ARFAScoreManager* ScoreManager = MatchManager.GetScoreManager();
    const int32 RemainingSeconds = FMath::CeilToInt(MatchManager.GetRemainingTime());
    const int32 Minutes = RemainingSeconds / 60;
    const int32 Seconds = RemainingSeconds % 60;

    const FString ScoreText = FString::Printf(TEXT("Home %d  x  %d Away"), ScoreManager->GetHomeScore(), ScoreManager->GetAwayScore());
    const FString TimeText = FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds);

    DrawRect(FLinearColor(0.02f, 0.03f, 0.04f, 0.72f), 24.0f, 24.0f, 310.0f, 86.0f);
    DrawText(ScoreText, FLinearColor::White, 40.0f, 36.0f, nullptr, 1.55f, false);
    DrawText(TimeText, FLinearColor(0.85f, 0.95f, 1.0f, 1.0f), 40.0f, 75.0f, nullptr, 1.2f, false);
}

void ARFAHUD::DrawMatchStatus(const ARFAMatchManager& MatchManager)
{
    if (!Canvas)
    {
        return;
    }

    FString StatusText;
    if (MatchManager.IsWaitingForRestart() && MatchManager.HasScoringTeam())
    {
        const TCHAR* TeamName = MatchManager.GetLastScoringTeam() == ERedFootTeam::Home ? TEXT("Home") : TEXT("Away");
        StatusText = FString::Printf(TEXT("Goal %s  |  Restart %.1fs"), TeamName, MatchManager.GetRestartRemainingTime());
    }
    else if (MatchManager.IsMatchFinished())
    {
        if (MatchManager.IsMatchTied())
        {
            StatusText = TEXT("Full time  |  Draw");
        }
        else
        {
            const TCHAR* TeamName = MatchManager.GetWinningTeam() == ERedFootTeam::Home ? TEXT("Home") : TEXT("Away");
            StatusText = FString::Printf(TEXT("Full time  |  %s wins"), TeamName);
        }
    }

    if (StatusText.IsEmpty())
    {
        return;
    }

    const float PanelWidth = 440.0f;
    const float PanelX = (Canvas->SizeX - PanelWidth) * 0.5f;
    DrawRect(FLinearColor(0.02f, 0.03f, 0.04f, 0.76f), PanelX, 126.0f, PanelWidth, 56.0f);
    DrawText(StatusText, FLinearColor(1.0f, 0.92f, 0.55f, 1.0f), PanelX + 24.0f, 142.0f, nullptr, 1.2f, false);
}

void ARFAHUD::DrawStaminaBar(const URFAStaminaComponent& StaminaComponent)
{
    if (!Canvas)
    {
        return;
    }

    const float MaxStamina = FMath::Max(1.0f, StaminaComponent.GetMaxStamina());
    const float StaminaPercent = FMath::Clamp(StaminaComponent.GetCurrentStamina() / MaxStamina, 0.0f, 1.0f);
    const float BarX = 24.0f;
    const float BarY = Canvas->SizeY - 66.0f;
    const float BarWidth = 260.0f;
    const float BarHeight = 18.0f;

    DrawText(TEXT("Stamina"), FLinearColor::White, BarX, BarY - 26.0f, nullptr, 1.0f, false);
    DrawRect(FLinearColor(0.02f, 0.03f, 0.04f, 0.72f), BarX - 8.0f, BarY - 8.0f, BarWidth + 16.0f, BarHeight + 16.0f);
    DrawRect(FLinearColor(0.08f, 0.11f, 0.12f, 1.0f), BarX, BarY, BarWidth, BarHeight);
    DrawRect(FLinearColor(0.2f, 0.86f, 0.55f, 1.0f), BarX, BarY, BarWidth * StaminaPercent, BarHeight);
}

URFAStaminaComponent* ARFAHUD::FindPlayerStamina() const
{
    if (!PlayerOwner)
    {
        return nullptr;
    }

    const ARFAPlayerCharacter* PlayerCharacter = Cast<ARFAPlayerCharacter>(PlayerOwner->GetPawn());
    if (!PlayerCharacter)
    {
        return nullptr;
    }

    return PlayerCharacter->GetStaminaComponent();
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
