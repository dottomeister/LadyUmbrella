//  
// MainHUD.h
// 
// HUD for the main game. 
// 
// Copyright Zulo Interactive. All Rights Reserved.
//

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "LadyUmbrella/UI/Menus/CheatMenu/CheatMenu.h"
#include "MainHUD.generated.h"

class APlayerCharacter;
class UPauseMenu;
class UPlayerHealth;
class UAmmoCounter;
class UCrosshair;
class USettingsMenu;
class UControlSettings;
class USubtitles;
class UGrenadeIndicator;
class AGrenade;

/**
 * 
 */
UCLASS()
class LADYUMBRELLA_API AMainHUD : public AHUD
{
	GENERATED_BODY()

	UPROPERTY()
	UPauseMenu* PauseMenuInstance;

	UPROPERTY()
	UPlayerHealth* PlayerHealthInstance;

	UPROPERTY()
	UAmmoCounter* AmmoCounterInstance;

	UPROPERTY()
	UCrosshair* CrosshairInstance;

	UPROPERTY()
	UCheatMenu* CheatMenuInstance;

	UPROPERTY()
	USettingsMenu* SettingsMenuInstance;

	UPROPERTY()
	USubtitles* SubtitlesInstance;

	UPROPERTY()
	UGrenadeIndicator* GrenadeIndicatorInstance;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Menus", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UPauseMenu> PauseMenuWidget;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Menus", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UCheatMenu> CheatMenuWidget;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Menus", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<USettingsMenu> SettingsMenuWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SubMenus", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UControlSettings> ControlSettingsMenuWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HUD", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UPlayerHealth> PlayerHealthWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HUD", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UAmmoCounter> AmmoCounterWidget;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HUD", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UCrosshair> CrosshairWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HUD", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<USubtitles> SubtitlesWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HUD", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UGrenadeIndicator> GrenadeIndicatorWidget;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HUD", meta = (AllowPrivateAccess = "true"))
	bool bPlayerHealthBar;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HUD", meta = (AllowPrivateAccess = "true"))
	float HidePlayerHUDTime;
	
	FTimerHandle HidePlayerHUDTimerHandle;
	
	bool bCheatMenuCanToggle;

	UPROPERTY()
	bool bIsOn;

public:

	UFUNCTION()
	void OnUmbrellaSpawned();

	UFUNCTION(BlueprintCallable) void HideEverythingExceptSubtitle() const;
	UFUNCTION(BlueprintCallable) void ShowEverythingExceptMenus() const;

	UFUNCTION(BlueprintCallable)
	void TogglePauseMenu() const;
	void ToggleCheatMenu();
	void EnableToggleCheatMenu();

	UFUNCTION(BlueprintCallable)
	void ToggleSettingsMenu() const;
	
	UFUNCTION(BlueprintCallable)
	void TogglePlayerHUD();

	UFUNCTION(BlueprintCallable)
	void UnTogglePlayerHUD();

	UFUNCTION(BlueprintCallable)
	void ToggleGrenadeIndicator(AGrenade* Grenade) const;

	UFUNCTION(BlueprintCallable)
	void UnToggleGrenadeIndicator() const;

	void ClearWidgets();
	void AddSubtitle(const FString& CharacterName, const FString& Subtitle,const float& Duration) const;
	void RemoveSubtitle(const FString& CharacterName) const;
	void HidePlayerHUDOnTimer();
	void ResetHidePlayerHUDTimer();

	UPauseMenu* GetPauseMenuInstance() const;
	TSubclassOf<UPauseMenu> GetPauseMenuWidget() const;
	UPlayerHealth* GetPlayerHealthInstance() const;
	TSubclassOf<UPlayerHealth> GetPlayerHealthWidget() const;
	UAmmoCounter* GetAmmoCounterInstance() const;
	TSubclassOf<UAmmoCounter> GetAmmoCounterWidget() const;
	UCrosshair* GetCrosshairInstance() const;
	TSubclassOf<UCrosshair> GetCrosshairWidget() const;
	UCheatMenu* GetCheatMenuInstance() const;
	TSubclassOf<UCheatMenu> GetCheatMenuWidget() const;
	USettingsMenu* GetSettingsMenuInstance() const;
	TSubclassOf<USettingsMenu> GetSettingsMenuWidget() const;
	TSubclassOf<UControlSettings> GetControlSettingsWidget() const;
	USubtitles* GetSubtitlesInstance() const;
	TSubclassOf<USubtitles> GetSubtitlesWidget() const;
	UGrenadeIndicator* GetGrenadeIndicatorInstance() const;
	TSubclassOf<UGrenadeIndicator> GetGrenadeIndicatorWidget() const;
	bool GetPlayerHUDState() const;
	
	void SetPauseMenuInstance(const TSubclassOf<UPauseMenu>& NewPauseMenuWidget);
	void SetCheatMenuInstance(const TSubclassOf<UCheatMenu>& NewCheatMenuWidget);
	void SetPlayerHealthInstance(const TSubclassOf<UPlayerHealth>& NewPlayerHealthWidget);
	void SetAmmoCounterInstance(const TSubclassOf<UAmmoCounter>& NewAmmoCounterWidget);
	void SetCrosshairInstance(const TSubclassOf<UCrosshair>& NewCrosshairWidget);
	void SetSettingsMenuInstance(const TSubclassOf<USettingsMenu>& NewSettingsMenuWidget);
	void SetSubtitleInstance(const TSubclassOf<USubtitles>& NewSubtitleWidget);
	void SetGrenadeIndicatorInstance(const TSubclassOf<UGrenadeIndicator>& NewGrenadeIndicatorWidget);
	void SetPlayerHUDState(const bool State);

protected:

	virtual void BeginPlay() override;
	
};
