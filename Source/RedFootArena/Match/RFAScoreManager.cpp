#include "Match/RFAScoreManager.h"

ARFAScoreManager::ARFAScoreManager()
{
    PrimaryActorTick.bCanEverTick = false;
}

void ARFAScoreManager::AddGoal(ERedFootTeam ScoringTeam)
{
    if (ScoringTeam == ERedFootTeam::Home)
    {
        ++HomeScore;
    }
    else
    {
        ++AwayScore;
    }

    OnScoreChanged.Broadcast(HomeScore, AwayScore);
}

void ARFAScoreManager::ResetScore()
{
    HomeScore = 0;
    AwayScore = 0;
    OnScoreChanged.Broadcast(HomeScore, AwayScore);
}

