// 
// MainHUD.cpp
// 
// Implementation of the MainHUD class. 
// 
// Copyright Zulo Interactive. All Rights Reserved.
// 

#include "MainHUD.h"
#include "Blueprint/UserWidget.h"
#include "LadyUmbrella/UI/HUD/AmmoCounter.h"
#include "LadyUmbrella/UI/HUD/Crosshair.h"
#include "LadyUmbrella/UI/HUD/PlayerHealth.h"
#include "LadyUmbrella/UI/HUD/Subtitles.h"
#include "LadyUmbrella/UI/Menus/PauseMenu/PauseMenu.h"
#include "LadyUmbrella/UI/Menus/CheatMenu/CheatMenu.h"
#include "LadyUmbrella/UI/Menus/SettingsMenu/Section/ControlSettings.h"
#include "LadyUmbrella/UI/Menus/SettingsMenu/SettingsMenu.h"
#include "LadyUmbrella/UI/HUD/GrenadeIndicator.h"

void AMainHUD::BeginPlay()
{
	Super::BeginPlay();
	
	SetPauseMenuInstance(GetPauseMenuWidget());
	SetCheatMenuInstance(GetCheatMenuWidget());
	SetSettingsMenuInstance(GetSettingsMenuWidget());
	SetPlayerHealthInstance(GetPlayerHealthWidget());
	SetSubtitleInstance(GetSubtitlesWidget());
	SetGrenadeIndicatorInstance(GetGrenadeIndicatorWidget());
	
	GetGrenadeIndicatorInstance()->AddToViewport();
	GetGrenadeIndicatorInstance()->SetVisibility(ESlateVisibility::Hidden);
	
	GetPlayerHealthInstance()->AddToViewport();
	GetPlayerHealthInstance()->SetVisibility(ESlateVisibility::Hidden);
	GetSubtitlesInstance()->AddToViewport();
}

void AMainHUD::OnUmbrellaSpawned()
{
	SetAmmoCounterInstance(GetAmmoCounterWidget());
	GetAmmoCounterInstance()->AddToViewport();
	SetCrosshairInstance(GetCrosshairWidget());
	GetCrosshairInstance()->AddToViewport();
	TogglePlayerHUD();
}

void AMainHUD::HideEverythingExceptSubtitle() const
{
	GetAmmoCounterInstance()->SetVisibility(ESlateVisibility::Hidden);
	GetCrosshairInstance()->SetVisibility(ESlateVisibility::Hidden);
}

void AMainHUD::ShowEverythingExceptMenus() const
{
	GetAmmoCounterInstance()->SetVisibility(ESlateVisibility::Visible);
	GetCrosshairInstance()->SetVisibility(ESlateVisibility::Visible);
	GetSubtitlesInstance()->SetVisibility(ESlateVisibility::Visible);
}

void AMainHUD::TogglePauseMenu() const
{
	if (IsValid(GetPauseMenuInstance()))
	{
		GetPauseMenuInstance()->Show();
	}
}

void AMainHUD::ToggleCheatMenu()
{
	if (IsValid(GetCheatMenuInstance()) && bCheatMenuCanToggle)
	{
		if (GetCheatMenuInstance()->IsVisible())
		{
			GetCheatMenuInstance()->Hide();
		}
		else
		{
			GetCheatMenuInstance()->Show();
		}

		bCheatMenuCanToggle = false;
	}
}

void AMainHUD::EnableToggleCheatMenu()
{
	bCheatMenuCanToggle = true;
}

void AMainHUD::ToggleSettingsMenu() const
{
	if (IsValid(GetSettingsMenuInstance()))
	{
		GetSettingsMenuInstance()->Show();
	}
}

void AMainHUD::TogglePlayerHUD()
{
	if ( IsValid(GetPlayerHealthInstance()) && bPlayerHealthBar)
	{
		GetPlayerHealthInstance()->Show();
	}
	if (IsValid(GetAmmoCounterInstance()) && IsValid(GetCrosshairInstance()))
	{
		GetAmmoCounterInstance()->Show();
		GetCrosshairInstance()->Show();
	}
	SetPlayerHUDState(true);
}

void AMainHUD::UnTogglePlayerHUD()
{
	if ( IsValid(GetPlayerHealthInstance()) && bPlayerHealthBar)
	{
		GetPlayerHealthInstance()->Hide();
	}
	if (IsValid(GetAmmoCounterInstance()) && IsValid(GetCrosshairInstance()))
	{
		GetAmmoCounterInstance()->Hide();
		GetCrosshairInstance()->Hide();
	}
	SetPlayerHUDState(false);
}


void AMainHUD::ToggleGrenadeIndicator(AGrenade* Grenade) const
{
	if (IsValid(GetGrenadeIndicatorInstance()))
	{
		GetGrenadeIndicatorInstance()->Show(Grenade);
	}
}

void AMainHUD::UnToggleGrenadeIndicator() const
{
	if (IsValid(GetGrenadeIndicatorInstance()))
	{
		GetGrenadeIndicatorInstance()->Hide();
	}
}

void AMainHUD::ClearWidgets()
{
	if (GetPauseMenuInstance())
	{
		GetPauseMenuInstance()->RemoveFromParent();
		SetPauseMenuInstance(nullptr);
	}

	if (GetSettingsMenuInstance())
	{
		GetSettingsMenuInstance()->RemoveFromParent();
		SetSettingsMenuInstance(nullptr);
	}

	if (GetPlayerHealthInstance())
	{
		GetPlayerHealthInstance()->RemoveFromParent();
		SetPlayerHealthInstance(nullptr);
	}

	if (GetAmmoCounterInstance())
	{
		GetAmmoCounterInstance()->RemoveFromParent();
		SetAmmoCounterInstance(nullptr);
	}

	if (GetCrosshairInstance())
	{
		GetCrosshairInstance()->RemoveFromParent();
		SetCrosshairInstance(nullptr);
	}

	if (GetSubtitlesInstance())
	{
		GetSubtitlesInstance()->RemoveFromParent();
		SetSubtitleInstance(nullptr);
	}
}

void AMainHUD::AddSubtitle(const FString& CharacterName, const FString& Subtitle,const float& Duration) const
{
	GetSubtitlesInstance()->AddSubtitle(CharacterName, Subtitle, Duration);
}

void AMainHUD::RemoveSubtitle(const FString& CharacterName) const
{
	GetSubtitlesInstance()->RemoveSubtitle(CharacterName);
}

void AMainHUD::HidePlayerHUDOnTimer()
{
	GetWorldTimerManager().SetTimer(
		HidePlayerHUDTimerHandle,
		this,
		&AMainHUD::UnTogglePlayerHUD,
		HidePlayerHUDTime,
		false,
		HidePlayerHUDTime
	);
}

void AMainHUD::ResetHidePlayerHUDTimer()
{
	if (GetWorldTimerManager().IsTimerActive(HidePlayerHUDTimerHandle))
	{
		GetWorldTimerManager().ClearTimer(HidePlayerHUDTimerHandle);
	}
}

UPauseMenu* AMainHUD::GetPauseMenuInstance() const 
{
	return PauseMenuInstance;
}

UCheatMenu* AMainHUD::GetCheatMenuInstance() const 
{
	return CheatMenuInstance;
}

TSubclassOf<UCheatMenu> AMainHUD::GetCheatMenuWidget() const
{
	return CheatMenuWidget;
}

UPlayerHealth* AMainHUD::GetPlayerHealthInstance() const
{
	return PlayerHealthInstance;
}

TSubclassOf<UPlayerHealth> AMainHUD::GetPlayerHealthWidget() const
{
	return PlayerHealthWidget;
}

UAmmoCounter* AMainHUD::GetAmmoCounterInstance() const
{
	return AmmoCounterInstance;
}

TSubclassOf<UAmmoCounter> AMainHUD::GetAmmoCounterWidget() const
{
	return AmmoCounterWidget;
}

UCrosshair* AMainHUD::GetCrosshairInstance() const
{
	return CrosshairInstance;
}

TSubclassOf<UCrosshair> AMainHUD::GetCrosshairWidget() const
{
	return CrosshairWidget;
}

TSubclassOf<UPauseMenu> AMainHUD::GetPauseMenuWidget() const
{
	return PauseMenuWidget;
}

USettingsMenu* AMainHUD::GetSettingsMenuInstance() const
{
	return SettingsMenuInstance;
}

TSubclassOf<USettingsMenu>  AMainHUD::GetSettingsMenuWidget() const
{
	return SettingsMenuWidget;
}

TSubclassOf<UControlSettings>  AMainHUD::GetControlSettingsWidget() const
{
	return ControlSettingsMenuWidget;
}

USubtitles* AMainHUD::GetSubtitlesInstance() const
{
	return SubtitlesInstance;
}

TSubclassOf<USubtitles> AMainHUD::GetSubtitlesWidget() const
{
	return SubtitlesWidget;
}

UGrenadeIndicator* AMainHUD::GetGrenadeIndicatorInstance() const
{
	return GrenadeIndicatorInstance;
}

TSubclassOf<UGrenadeIndicator> AMainHUD::GetGrenadeIndicatorWidget() const
{
	return GrenadeIndicatorWidget;
}

bool AMainHUD::GetPlayerHUDState() const
{
	return bIsOn;
}

void AMainHUD::SetPauseMenuInstance(const TSubclassOf<UPauseMenu>& NewPauseMenuWidget)
{
	if (!IsValid(GetPauseMenuInstance()) && IsValid(NewPauseMenuWidget))
	{
		PauseMenuInstance = CreateWidget<UPauseMenu>(GetWorld(), NewPauseMenuWidget);
	}
}

void AMainHUD::SetCheatMenuInstance(const TSubclassOf<UCheatMenu>& NewCheatMenuWidget)
{
	if (!IsValid(GetCheatMenuInstance()) && IsValid(NewCheatMenuWidget))
	{
		CheatMenuInstance = CreateWidget<UCheatMenu>(GetWorld(), NewCheatMenuWidget);
	}
}

void AMainHUD::SetPlayerHealthInstance(const TSubclassOf<UPlayerHealth>& NewPlayerHealthWidget)
{
	if (!IsValid(GetPlayerHealthInstance()) && IsValid(NewPlayerHealthWidget))
	{
		PlayerHealthInstance = CreateWidget<UPlayerHealth>(GetWorld(), NewPlayerHealthWidget);
	}
}

void AMainHUD::SetAmmoCounterInstance(const TSubclassOf<UAmmoCounter>& NewAmmoCounterWidget)
{
	if (!IsValid(GetAmmoCounterInstance()) && IsValid(NewAmmoCounterWidget))
	{
		AmmoCounterInstance = CreateWidget<UAmmoCounter>(GetWorld(), NewAmmoCounterWidget);
	}
}

void AMainHUD::SetCrosshairInstance(const TSubclassOf<UCrosshair>& NewCrosshairWidget)
{
	if (!IsValid(GetCrosshairInstance()) && IsValid(NewCrosshairWidget))
	{
		CrosshairInstance = CreateWidget<UCrosshair>(GetWorld(), NewCrosshairWidget);
	}
}

void AMainHUD::SetSettingsMenuInstance(const TSubclassOf<USettingsMenu>& NewSettingsMenuWidget)
{
	if (!IsValid(GetSettingsMenuInstance()) && IsValid(NewSettingsMenuWidget))
	{
		SettingsMenuInstance = CreateWidget<USettingsMenu>(GetWorld(), NewSettingsMenuWidget);
		if (IsValid(SettingsMenuInstance))
		{
			SettingsMenuInstance->LoadGame();
		}
	}
}

void AMainHUD::SetSubtitleInstance(const TSubclassOf<USubtitles>& NewSubtitleWidget)
{
	if (!IsValid(GetSubtitlesInstance()) && IsValid(NewSubtitleWidget))
	{
		SubtitlesInstance = CreateWidget<USubtitles>(GetWorld(), NewSubtitleWidget);
	}
}

void AMainHUD::SetGrenadeIndicatorInstance(const TSubclassOf<UGrenadeIndicator>& NewGrenadeIndicatorWidget)
{
	if (!IsValid(GetGrenadeIndicatorInstance()) && IsValid(NewGrenadeIndicatorWidget))
	{
		GrenadeIndicatorInstance = CreateWidget<UGrenadeIndicator>(GetWorld(), NewGrenadeIndicatorWidget);
	}
}

void AMainHUD::SetPlayerHUDState(const bool State)
{
	bIsOn = State;
}
