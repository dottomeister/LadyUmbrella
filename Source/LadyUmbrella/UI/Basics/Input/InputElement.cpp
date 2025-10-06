#include "InputElement.h"
#include "InputType/SelectorInput.h"
#include "InputType/SliderInput.h"
#include "InputType/ToggleInput.h"
#include "LadyUmbrella/Assets/WidgetAssets.h"

void UInputElement::NativeConstruct()
{
	Super::NativeConstruct();
	UpdateSectionText();
}

void UInputElement::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	if (!IsValid(InputWidgetInstance))
	{
		return;
	}
	
	UpdateSectionText();
	if (!SelectionList.IsEmpty() && InputType == EInputElementType::SELECTOR && InputWidgetInstance->IsA(USelectorInput::StaticClass()))
	{
		USelectorInput* Widget = Cast<USelectorInput>(InputWidgetInstance);
		Widget->SetOptions(SelectionList);
		Widget->UpdateOptions();
	}
}

FReply UInputElement::NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent)
{
	PlayAnimation(FocusAnim, FWidgetAssets::START_AT_BEGINNING, FWidgetAssets::SINGLE_PLAY_ANIMATION, EUMGSequencePlayMode::Forward);
	return Super::NativeOnFocusReceived(InGeometry, InFocusEvent);
}

void UInputElement::NativeOnFocusLost(const FFocusEvent& InFocusEvent)
{
	Super::NativeOnFocusLost(InFocusEvent);
	PlayAnimation(FocusAnim, FWidgetAssets::START_AT_BEGINNING, FWidgetAssets::SINGLE_PLAY_ANIMATION, EUMGSequencePlayMode::Reverse);
}


bool UInputElement::HandleInput(bool bInteractLeft)
{
	if (!HasKeyboardFocus() || InputWidgetInstance->IsPlayingAnimation())
	{
		return false;
	}
	
	switch (InputType)
	{
		case EInputElementType::SELECTOR:
			if (bInteractLeft)
			{
				Cast<USelectorInput>(InputWidgetInstance)->SwitchPrev();
				return true;
			}
		
			Cast<USelectorInput>(InputWidgetInstance)->SwitchNext();
			return true;
		
		case EInputElementType::SLIDER:
		{
			USliderInput* Slider = Cast<USliderInput>(InputWidgetInstance);
			
			Slider->UpdateValue(bInteractLeft);
			Slider->UpdateText();
			Slider->UpdateStep();
			return true;
		}
		case EInputElementType::TOGGLABLE:
		{
			UToggleInput* Toggle = Cast<UToggleInput>(InputWidgetInstance);

			if ((bInteractLeft && !Toggle->GetStatus()) || (!bInteractLeft && Toggle->GetStatus()))
			{
				Toggle->UpdateOptions();
				return true;
			}
			break;
		}
	}
	
	return false;
}

void UInputElement::UpdateSectionText()
{
	Title->SetText(!Text.IsEmpty() ? FText::FromString(Text) : FText::FromString("Error"));
}
