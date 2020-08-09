// Copyright (c) 2020 Epgenix


#include "ReplaySystemAdvancedPC.h"
#include "Engine/World.h"
#include "Engine/DemoNetDriver.h"

AReplaySystemAdvancedPC::AReplaySystemAdvancedPC(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
	bShowMouseCursor = true; PrimaryActorTick.bTickEvenWhenPaused = true; bShouldPerformFullTickWhenPaused = true;
}

bool AReplaySystemAdvancedPC::SetCurrentReplayPausedState(bool bDoPause)
{
	AWorldSettings* WorldSettings = GetWorldSettings();
	// Set MotionBlur off and Anti Aliasing to FXAA in order to bypass the pause-bug of both
	static const auto CVarAA = IConsoleManager::Get().FindConsoleVariable(TEXT("r.DefaultFeature.AntiAliasing"));

	static const auto CVarMB = IConsoleManager::Get().FindConsoleVariable(TEXT("r.DefaultFeature.MotionBlur"));

	if (bDoPause)
	{
		PreviousAASetting = CVarAA->GetInt();
		PreviousMBSetting = CVarMB->GetInt();

		// Set MotionBlur to OFF, Anti-Aliasing to FXAA
		CVarAA->Set(1);
		CVarMB->Set(0);

		WorldSettings->SetPauserPlayerState(PlayerState);
		return true;
	}
	// Rest MotionBlur and AA
	CVarAA->Set(PreviousAASetting);
	CVarMB->Set(PreviousMBSetting);

	WorldSettings->SetPauserPlayerState(NULL);
	return false;
}


int32 AReplaySystemAdvancedPC::GetCurrentReplayTotalTimeInSeconds() const
{
	if (!GetWorld() || !GetWorld()->DemoNetDriver)
		return 0;
	return GetWorld()->DemoNetDriver->DemoTotalTime;
}

int32 AReplaySystemAdvancedPC::GetCurrentReplayCurrentTimeInSeconds() const
{
	if (!GetWorld() || !GetWorld()->DemoNetDriver)
		return 0;
	return GetWorld()->DemoNetDriver->DemoCurrentTime;
}

	void AReplaySystemAdvancedPC::SetCurrentReplayTimeToSeconds(int32 Seconds)
{
	if (GetWorld())
	{	
			if (GetWorld()->DemoNetDriver)
			{
				GetWorld()->DemoNetDriver->GotoTimeInSeconds(Seconds);
			}
	}
}
void AReplaySystemAdvancedPC::SetCurrentReplayPlayRate(float PlayRate)
{
	if (GetWorld())
	{
		if (GetWorld()->DemoNetDriver)
		{
			GetWorld()->GetWorldSettings()->DemoPlayTimeDilation = PlayRate;
		}
	}
}

