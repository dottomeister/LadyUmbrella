// 
// MainController.h
// 
// MainController for the main game. 
// 
// Copyright Zulo Interactive. All Rights Reserved.
//

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "LadyUmbrella/UI/HUD/MainHUD.h"
#include "LadyUmbrella/Util/Localization/LanguageCode.h"
#include "MainController.generated.h"

class UTimeDilationComponent;
class AUmbrella;
class AMainHUD;
class UInputAction;
class UInputMappingContext;
class AGrenade;

enum class ELanguageCode : uint8;

UCLASS()
class LADYUMBRELLA_API AMainController : public APlayerController
{
	GENERATED_BODY()

	// +-------------------------------------------------+
	// |                                                 |
	// |                    VARIABLES                    |
	// |                                                 |
	// +-------------------------------------------------+
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Localization", meta = (AllowPrivateAccess = "true"))
	bool bSubtitlesEnabled;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Localization", meta = (AllowPrivateAccess = "true"))
	ELanguageCode LanguageCode;

	UPROPERTY()
	TObjectPtr<AMainHUD> MainHUD;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UTimeDilationComponent> TimeDilationComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	FString InputDevice;

public:
	
	// +-------------------------------------------------+
	// |                                                 |
	// |                    FUNCTIONS                    |
	// |                                                 |
	// +-------------------------------------------------+
	AMainController();

protected:

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void SetupInputComponent() override;

public:

	void ParseInputDevice();
	UFUNCTION(BlueprintCallable) ELanguageCode GetNextLanguageCode();

	// NOTE: This function CANNOT be made const because it is used in the UEnhancedInput delegate.
	void TogglePauseMenu();

	void ClearWidgets() const;
	void ToggleSettingsMenu() const;
	UFUNCTION(BlueprintCallable) void TogglePlayerHUD() const;
	UFUNCTION() void UmbrellaSpawned() const;
	void HandleToggleGrenadeIndicator(AGrenade* Grenade) const;
	void HandleUnToggleGrenadeIndicator() const;
	UFUNCTION(BlueprintCallable) void UnTogglePlayerHUD() const;
	void DeactivatePlayerHUDOnTimer() const;
	void ActivateResetHidePlayerHUDTimer() const;
	void ShowPlayerHUDOnKeyPressed();
	
	// +-------------------------------------------------+
	// |                                                 |
	// |                GETTERS & SETTERS                |
	// |                                                 |
	// +-------------------------------------------------+
	FORCEINLINE ELanguageCode GetLanguageCode() const { return LanguageCode; }
	FORCEINLINE TObjectPtr<AMainHUD> GetMainHUD() const { return MainHUD; }
	FORCEINLINE TObjectPtr<UTimeDilationComponent> GetTimeDilationComponent() { return TimeDilationComponent; }
	FORCEINLINE FString GetInputDevice() const { return InputDevice; }
	FORCEINLINE bool IsSubtitlesEnabled() const { return bSubtitlesEnabled; }

	UFUNCTION(BlueprintCallable) FORCEINLINE void SetLanguageCode(const ELanguageCode Value) { LanguageCode = Value; }
	FORCEINLINE void SetMainHUD(const TObjectPtr<AMainHUD>& Value) { MainHUD = Value; }
	FORCEINLINE void SetInputDevice(const FString& Value) { InputDevice = Value; }

	// +-------------------------------------------------+
	// |                                                 |
	// |                DEVELOPMENT STUFF                |
	// |                                                 |
	// +-------------------------------------------------+
	int32 ActiveControllerInputMappingContextIndex;
	int32 ActiveKeyboardInputMappingContextIndex;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Development")
	float NoClipSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input Mapping Contexts", meta = (AllowPrivateAccess = "true"))
	TArray<UInputMappingContext*> ControllerInputMappingContexts;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input Mapping Contexts", meta = (AllowPrivateAccess = "true"))
	TArray<UInputMappingContext*> KeyboardInputMappingContexts;
	
	void ActivateNextControllerInputMappingContext();
	void ActivateNextKeyboardInputMappingContext();
	
	UFUNCTION() TWeakObjectPtr<UInputMappingContext> GetControllerMappingContextAtIndex(int32 _InputMappingContextIndex) const;
	UFUNCTION() TWeakObjectPtr<UInputMappingContext> GetKeyboardMappingContextAtIndex(int32 _InputMappingContextIndex) const;

	FORCEINLINE int32 GetControllerInputMappingContextIndex() const { return ActiveControllerInputMappingContextIndex; }
	FORCEINLINE int32 GetKeyboardInputMappingContextIndex() const { return ActiveKeyboardInputMappingContextIndex; }
	
	void SetActiveControllerInputMappingContext(int _NewInputMappingContextIndex);
	void SetActiveKeyboardInputMappingContext(int _NewInputMappingContextIndex);
	
	void ToggleCheatMenu();
	void EnableToggleCheatMenu();
	UFUNCTION(Exec) void Godmode() const;
	UFUNCTION(Exec) void NoClip() const;
	UFUNCTION(Exec) void InfiniteAmmo();
	UFUNCTION(Exec) void ReloadLevel();
	UFUNCTION(Exec) void SetAmmoCurrent(const int Amount);
	UFUNCTION(Exec) void SetAmmoReserve(const int Amount);
	UFUNCTION(Exec) void SetComponents(const int Amount);
	UFUNCTION(Exec) void UnlockGadgets();
	UFUNCTION(Exec) void UnlockUpgrades();
};
