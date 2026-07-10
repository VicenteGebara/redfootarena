#include "Audio/RFAAudioManager.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"
#include "UObject/ConstructorHelpers.h"

ARFAAudioManager::ARFAAudioManager()
{
    PrimaryActorTick.bCanEverTick = false;

    static ConstructorHelpers::FObjectFinder<USoundBase> PingAsset(TEXT("/Engine/EngineSounds/1kSineTonePing.1kSineTonePing"));
    if (PingAsset.Succeeded())
    {
        PingSound = PingAsset.Object;
    }

    static ConstructorHelpers::FObjectFinder<USoundBase> NoiseAsset(TEXT("/Engine/EngineSounds/WhiteNoise.WhiteNoise"));
    if (NoiseAsset.Succeeded())
    {
        NoiseSound = NoiseAsset.Object;
    }
}

void ARFAAudioManager::PlayKick()
{
    PlayNoise(0.16f, 1.85f);
    PlayPing(0.22f, 0.62f);
}

void ARFAAudioManager::PlayPass()
{
    PlayPing(0.22f, 0.92f);
}

void ARFAAudioManager::PlayGoal()
{
    PlayPing(0.6f, 0.5f);
    PlayNoise(0.11f, 0.55f);
}

void ARFAAudioManager::PlayKickoff()
{
    PlayPing(0.25f, 1.24f);
}

void ARFAAudioManager::PlayReset()
{
    PlayPing(0.18f, 0.72f);
}

void ARFAAudioManager::PlayMatchEnd()
{
    PlayPing(0.42f, 0.42f);
}

void ARFAAudioManager::HandleGoalScored(ERedFootTeam ScoringTeam, float RestartDelay)
{
    PlayGoal();
}

void ARFAAudioManager::HandleMatchStarted()
{
    PlayKickoff();
}

void ARFAAudioManager::HandleMatchEnded(ERedFootTeam WinningTeam)
{
    PlayMatchEnd();
}

void ARFAAudioManager::HandlePlayReset()
{
    PlayReset();
}

void ARFAAudioManager::PlaySound2D(USoundBase* Sound, float VolumeMultiplier, float PitchMultiplier) const
{
    if (!Sound)
    {
        return;
    }

    UGameplayStatics::PlaySound2D(this, Sound, VolumeMultiplier, PitchMultiplier);
}

void ARFAAudioManager::PlayPing(float VolumeMultiplier, float PitchMultiplier) const
{
    PlaySound2D(PingSound, VolumeMultiplier, PitchMultiplier);
}

void ARFAAudioManager::PlayNoise(float VolumeMultiplier, float PitchMultiplier) const
{
    PlaySound2D(NoiseSound, VolumeMultiplier, PitchMultiplier);
}

