#pragma once

enum ESteamAchievement : int;

#define STEAM_ENABLED 1

namespace Steam
{
	void Initialize();
	bool IsSteamAvailable();
	bool UnlockAchievement(const ESteamAchievement SteamAchievement);
};
