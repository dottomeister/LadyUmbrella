
#include "Steam.h"

#include "SteamAchievement.h"
#include "Steam/steam_api.h"
#include "LadyUmbrella/Util/LoggerUtil.h"

bool bAPISuccess = false;

void Steam::Initialize()
{
#if STEAM_ENABLED
	bAPISuccess = SteamAPI_Init();

	if (bAPISuccess)
		FLogger::DebugLog("Steam::Initialize was a success!");
#else
	FLogger::DebugLog("Steam is not running on this machine, so Steamworks will not be initialized!");
#endif // STEAM_ENABLED
}

bool Steam::IsSteamAvailable()
{
	return bAPISuccess;
}

bool Steam::UnlockAchievement(const ESteamAchievement SteamAchievement)
{
	const char* AchievementID = ToString(SteamAchievement);
	FLogger::DebugLog("Attempting to trigger achievement: (%s)", AchievementID);
	
	if (!bAPISuccess || !SteamUserStats())
	{
		FLogger::ErrorLog("Unable to connect with Steam! Steam.UnlockAchievement()");
		return false;
	}
	
	if (SteamUserStats()->SetAchievement(AchievementID))
	{
	 	FLogger::DebugLog("Achievement was successfully unlocked!");
		SteamUserStats()->StoreStats();
		return true;
	}
	
	return false;
}

