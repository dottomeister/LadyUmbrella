// 
// AnimatedButton.h
// 
// Animated Button for UI. 
// 
// Copyright Zulo Interactive. All Rights Reserved.
//

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "LadyUmbrella/UI/Basics/State/UIState.h"
#include "AnimatedBaseButton.generated.h"

class UButton;
class UImage;
class UOverlay;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnButtonActivated);

UCLASS()
class LADYUMBRELLA_API UAnimatedBaseButton : public UUserWidget
{
	GENERATED_BODY()
protected:

	// Bindings
	UPROPERTY(BlueprintReadWrite, Category = "Widgets", meta = (BindWidget, AllowPrivateAccess = "true"))
	UButton* Button;

	UPROPERTY(BlueprintReadWrite, Category = "Widgets", meta = (BindWidget, AllowPrivateAccess = "true"))
	UTextBlock* ButtonName;
	
	// Variables
	UPROPERTY(EditAnywhere, Category="Customizable Button Values", meta = (AllowPrivateAccess = true))
	FString Text = "";
	
	UPROPERTY(EditAnywhere, Category="Customizable Button Values", meta = (AllowPrivateAccess = true))
	EUIState State = EUIState::Idle;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Customizable Button Values", meta = (AllowPrivateAccess = "true"))
	FName ButtonIdentifier;
	
public:
	
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnButtonActivated OnButtonActivated;

	TObjectPtr<UButton> GetButton() const { return Button; };
	FORCEINLINE FName GetIdentifier() const { return ButtonIdentifier; };
	FString GetText() const { return ButtonName->GetText().ToString(); }
	void SetButtonText(const FText& NewText) { ButtonName->SetText(NewText); }
	FORCEINLINE void SetTextVar(const FString& NewText) { Text = NewText; }
	FORCEINLINE EUIState GetState() const { return State; }
	FORCEINLINE void SetState(EUIState NewState) { State = NewState; }
	void UpdateButton();
	void SwapNormalPressedBrush();
	void ResetSwappedBrush();

protected:

	UPROPERTY()
	bool bBrushesSwapped = false;
	
	virtual void NativeConstruct() override;
	virtual void NativeOnInitialized() override;
	virtual FReply NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent) override;
	virtual void NativeOnFocusLost(const FFocusEvent& InFocusEvent) override;
	void UpdateButtonColor(FLinearColor NewColor) const;
	
	UFUNCTION()
	void GoToDisabled();
	
	UFUNCTION()
	void GoToIdle();
	
	UFUNCTION()
	void GoToHovered();
	
	UFUNCTION()
	void GoToFocused();
	
	UFUNCTION()
	virtual void GoToPressed();
	
	UFUNCTION()
	virtual void ActivateButton();
};
