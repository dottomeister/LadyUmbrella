#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "LadyUmbrella/UI/Basics/State/UIState.h"
#include "InputElement.generated.h"

class UOverlay;
class UImage;

UENUM()
enum class EInputElementType : uint8
{
	SELECTOR,
	TOGGLABLE,
	SLIDER,
	OTHER
};


UCLASS()
class LADYUMBRELLA_API UInputElement : public UUserWidget
{
	GENERATED_BODY()
	
	
	// Bindings
	UPROPERTY(BlueprintReadWrite, Category = "Widgets", meta = (BindWidget, AllowPrivateAccess = "true"))
	UOverlay* Section;
	
	UPROPERTY(BlueprintReadWrite, Category = "Widgets", meta = (BindWidget, AllowPrivateAccess = "true"))
	UTextBlock* Title;

	UPROPERTY(BlueprintReadWrite, Category = "Widgets", meta = (BindWidget, AllowPrivateAccess = "true"))
	UImage* SelectionOverlay;

	UPROPERTY(BlueprintReadWrite, Category = "Widgets", meta = (BindWidget, AllowPrivateAccess = "true"))
	UUserWidget* InputWidgetInstance;

	// Animations
	UPROPERTY(BlueprintReadWrite, Category = "Animation", meta = (BindWidgetAnim, AllowPrivateAccess = "true"), Transient)
	UWidgetAnimation* FocusAnim;

	//Text
	UPROPERTY(EditAnywhere, Category="Custom", meta = (AllowPrivateAccess = true))
	FString Text;
	
	UPROPERTY(EditAnywhere, Category="Custom", meta = (AllowPrivateAccess = true))
	FText Description;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Custom", meta = (AllowPrivateAccess = true))
	EInputElementType InputType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Custom", meta = (EditCondition = "InputType == EInputElementType::SELECTOR", EditConditionHides, AllowPrivateAccess = true))
	TArray<FString> SelectionList;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom", meta = (AllowPrivateAccess = "true"))
	FName InputIdentifier;
	
public:
	TObjectPtr<UOverlay> GetSection() const { return Section; }
	FORCEINLINE FName GetIdentifier() const { return InputIdentifier; }
	FORCEINLINE FString GetSectionText() const { return Title->GetText().ToString(); }
	FORCEINLINE void SetSectionText(const FText& NewText) { Title->SetText(NewText); }
	FORCEINLINE FString GetTitleText() const { return Text; }
	FORCEINLINE void SetTitleText(const FString NewText) { Text = NewText; }
	FORCEINLINE EUIState GetState() const { return State; }
	FORCEINLINE FText GetDescription() const { return Description; }
	FORCEINLINE void SetDescription(FText NewDescription) { Description = NewDescription; }
	FORCEINLINE void SetState(EUIState NewState) { State = NewState; }
	FORCEINLINE EInputElementType GetInputType() const { return InputType; }
	FORCEINLINE void SetInputType(EInputElementType NewType) { InputType = NewType; }
	FORCEINLINE TObjectPtr<UUserWidget> GetInputWidget() const { return InputWidgetInstance; }
	FORCEINLINE void SetInputWidget(TObjectPtr<UUserWidget> Widget) { InputWidgetInstance = Widget; }
	FORCEINLINE TArray<FString> GetSelectionList() const { return SelectionList; }

	bool HandleInput(bool bInteractLeft);
	void UpdateSectionText();

	float GetProcessedInputValue();
	
protected:
	UPROPERTY() EUIState State;
	
	virtual void NativeConstruct() override;
	virtual void NativeOnInitialized() override;
	virtual FReply NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent) override;
	virtual void NativeOnFocusLost(const FFocusEvent& InFocusEvent) override;
};

