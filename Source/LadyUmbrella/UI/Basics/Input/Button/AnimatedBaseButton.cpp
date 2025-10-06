// 
// AnimatedButton.cpp
// 
// Implementation of the AnimatedButton class.
// 
// Copyright Zulo Interactive. All Rights Reserved.
//
#include "LadyUmbrella/UI/Basics/Input/Button/AnimatedBaseButton.h"

#include "Animation/WidgetAnimation.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "LadyUmbrella/Assets/WidgetAssets.h"

void UAnimatedBaseButton::NativeOnInitialized() // this is called only once for the UUserWidget. If you have one-time things to establish up-front (like binding callbacks to events on BindWidget properties), do so here
{
	Super::NativeOnInitialized();
	
	Button->OnHovered.AddDynamic(this, &UAnimatedBaseButton::GoToHovered);
	Button->OnUnhovered.AddDynamic(this, &UAnimatedBaseButton::GoToIdle);
	Button->OnClicked.AddDynamic(this, &UAnimatedBaseButton::GoToPressed);
}

void UAnimatedBaseButton::NativeConstruct() // Should be used as Begin Play is used on regular Actors
{
	Super::NativeConstruct();
	
	ButtonName->SetTextTransformPolicy(ETextTransformPolicy::ToUpper);
	ButtonName->SetText(!Text.IsEmpty() ? FText::FromString(Text) : FText::FromString("Text Error"));

	UpdateButton();
}

void UAnimatedBaseButton::UpdateButton()
{
	bool bShouldBeFocusable = State != EUIState::Disabled;
	if (bShouldBeFocusable != IsFocusable())
	{
		SetIsFocusable(bShouldBeFocusable);
	}
	
	switch (State)
	{
		case EUIState::Pressed:
			GoToPressed();
		break;
		
		case EUIState::Hovered:
			GoToHovered();
		break;

		case EUIState::Disabled:
			GoToDisabled();
		break;
		
		default:
			GoToIdle();
		break;
	}
}

void UAnimatedBaseButton::GoToDisabled()
{
	SetState(EUIState::Disabled);
	SetIsEnabled(false);
}

void UAnimatedBaseButton::GoToIdle()
{
	if (!IsFocusable())
	{
		return;
	}
	
	SetState(EUIState::Idle);
	Button->SetVisibility(ESlateVisibility::Visible);
}

void UAnimatedBaseButton::GoToHovered()
{
	if (!IsFocusable())
	{
		return;
	}
	
	SetState(EUIState::Hovered);
}

void UAnimatedBaseButton::GoToFocused()
{
	if (!IsFocusable())
	{
		return;
	}
	
	SetState(EUIState::Hovered);
}

void UAnimatedBaseButton::GoToPressed()
{
	if (!IsFocusable())
	{
		return;
	}
	
	SetState(EUIState::Pressed);
	Button->SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UAnimatedBaseButton::ActivateButton()
{
	SetState(EUIState::Hovered);
	UpdateButton();
	OnButtonActivated.Broadcast();
}

FReply UAnimatedBaseButton::NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent)
{
	Super::NativeOnFocusReceived(InGeometry, InFocusEvent);
	if (State != EUIState::Hovered)
	{
		TSharedPtr<SWidget> SafeWidget = Button->GetCachedWidget();
		if (SafeWidget.IsValid())
		{
			SafeWidget->OnMouseEnter(InGeometry, FPointerEvent());
		}
	}

	return FReply::Handled();
}

void UAnimatedBaseButton::NativeOnFocusLost(const FFocusEvent& InFocusEvent)
{
	Super::NativeOnFocusLost(InFocusEvent);
	
	if (!IsFocusable())
	{
		return;
	}
	
	if (State != EUIState::Idle)
	{
		TSharedPtr<SWidget> SafeWidget = Button->GetCachedWidget();
		if (SafeWidget.IsValid())
		{
			SafeWidget->OnMouseLeave(FPointerEvent());
		}
	}
}

void UAnimatedBaseButton::SwapNormalPressedBrush()
{
	bBrushesSwapped = !bBrushesSwapped;
	FSlateBrush TempNormal = Button->GetStyle().Normal;
	FSlateBrush TempPress = Button->GetStyle().Pressed;
	FButtonStyle NewStyle = Button->GetStyle();
	NewStyle.SetPressed(TempNormal);
	NewStyle.SetNormal(TempPress);
	Button->SetStyle(NewStyle);
}

void UAnimatedBaseButton::ResetSwappedBrush()
{
	if (bBrushesSwapped)
	{
		SwapNormalPressedBrush();
	}
}

void UAnimatedBaseButton::UpdateButtonColor(const FLinearColor NewColor) const
{
	FSlateBrush CustomBrush = Button->GetStyle().Normal;
	CustomBrush.DrawAs = ESlateBrushDrawType::Box;
	CustomBrush.TintColor = FSlateColor(NewColor);
	FButtonStyle NewStyle = Button->GetStyle();
	NewStyle.SetNormal(CustomBrush);
	NewStyle.SetHovered(CustomBrush);
	NewStyle.SetDisabled(CustomBrush);
	NewStyle.SetPressed(CustomBrush);
	Button->SetStyle(NewStyle);
}