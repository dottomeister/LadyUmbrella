
#pragma once

UENUM(BlueprintType)
enum ESteamAchievement
{
	TEST_ACHIEVEMENT = 0 UMETA(DisplayName = "Steam Achievement")
};

static const char* ToString(const ESteamAchievement SteamAchievement)
{
	switch (SteamAchievement)
	{
	default:
	case TEST_ACHIEVEMENT:
		return "TEST_ACHIEVEMENT";
	}
}
