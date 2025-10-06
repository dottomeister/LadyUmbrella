//  
// MainMenu.h
// 
// Main Menu interface for the game. 
// 
// Copyright Zulo Interactive. All Rights Reserved.
//

#pragma once

#include "CoreMinimal.h"
#include "LadyUmbrella/UI/Menus/Menu.h"
#include "LadyUmbrella/UI/Basics/Input/Button/AnimatedMenuButton.h"
#include "MainMenu.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnButtonClicked);

class UVerticalBox;

UCLASS()
class LADYUMBRELLA_API UMainMenu : public UMenu
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float FadeSpeed = 1.5f;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetAnim, AllowPrivateAccess = "true"), Transient)
	UWidgetAnimation* Fade;
	
	UPROPERTY(BlueprintReadWrite, Category = "Buttons", meta = (BindWidget, AllowPrivateAccess = "true"))
	UAnimatedMenuButton* NewGameButton;
	
	UPROPERTY(BlueprintReadWrite, Category = "Buttons", meta = (BindWidget, AllowPrivateAccess = "true"))
	UAnimatedMenuButton* ContinueButton;

	UPROPERTY(BlueprintReadWrite, Category = "Buttons", meta = (BindWidget, AllowPrivateAccess = "true"))
	UAnimatedMenuButton* ChaptersMenuButton;

	UPROPERTY(BlueprintReadWrite, Category = "Buttons", meta = (BindWidget, AllowPrivateAccess = "true"))
	UAnimatedMenuButton* SettingsButton;
	
	UPROPERTY(BlueprintReadWrite, Category = "Buttons", meta = (BindWidget, AllowPrivateAccess = "true"))
	UAnimatedMenuButton* CreditsButton;
	
	UPROPERTY(BlueprintReadWrite, Category = "Buttons", meta = (BindWidget, AllowPrivateAccess = "true"))
	UAnimatedMenuButton* ExitGameButton;
	
public:
	
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnButtonClicked ButtonClickedDelegate;
	
	virtual void NativeOnInitialized() override;
	
	UFUNCTION(BlueprintCallable, Category = "Visibility")
	void Show();
	
	UFUNCTION(Blueprintable, BlueprintCallable, Category = "Functionality")
	void NewGame();

	UFUNCTION(BlueprintCallable, Category = "Functionality")
	void ShowChaptersMenu();
	
	UFUNCTION(BlueprintCallable, Category = "Functionality")
	void ExitDesktop();

	FORCEINLINE UAnimatedMenuButton* GetNewGameButton() const { return NewGameButton; }
	FORCEINLINE UAnimatedMenuButton* GetContinueButton() const { return ContinueButton; }
	FORCEINLINE UAnimatedMenuButton* GetChaptersMenuButton() const { return ChaptersMenuButton; }
	FORCEINLINE UAnimatedMenuButton* GetSettingsButton() const { return SettingsButton; }
	FORCEINLINE UAnimatedMenuButton* GetCreditsButton() const { return CreditsButton; }
	FORCEINLINE UAnimatedMenuButton* GetExitGameButton() const { return ExitGameButton; }

	FORCEINLINE void SetNewGameButton(UAnimatedMenuButton* Value) { NewGameButton = Value; }
	FORCEINLINE void SetContinueButton(UAnimatedMenuButton* Value) { NewGameButton = Value; }
	FORCEINLINE void SetChapterSelectorButton(UAnimatedMenuButton* Value) { NewGameButton = Value; }
	//FORCEINLINE void SetSettingsButton(UAnimatedButton* Value) { NewGameButton = Value; }
	FORCEINLINE void SetExitGameButton(UAnimatedMenuButton* Value) { ExitGameButton = Value; }
	
protected:
	UFUNCTION() void ContinueGame();
	UFUNCTION() void ShowCredits();
	UFUNCTION() void ShowSettings();

	virtual void NativeConstruct() override;
	virtual FReply NativeOnPreviewKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	
};