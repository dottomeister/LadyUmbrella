// 
// AnimatedButton.cpp
// 
// Implementation of the AnimatedButton class.
// 
// Copyright Zulo Interactive. All Rights Reserved.
//

#include "LadyUmbrella/UI/Basics/Input/Button/AnimatedMenuButton.h"
#include "Animation/WidgetAnimation.h"
#include "LadyUmbrella/Assets/WidgetAssets.h"

void UAnimatedMenuButton::GoToPressed()
{
	Super::GoToPressed();
	
	SwapNormalPressedBrush();

	if (AnimationSpeedMultiplier <= 0)
	{
		ActivateButton();
		return;
	}
	TScriptDelegate<> Delegate;
	Delegate.BindUFunction(this, FName("ActivateButton"));
	FWidgetAnimationDynamicEvent AnimFinishedEvent(Delegate);
	
	UnbindAllFromAnimationFinished(ClickedAnim);
	BindToAnimationFinished(ClickedAnim, AnimFinishedEvent);
	
	const float HalfAnimationSpeed = AnimationSpeedMultiplier / 2.f;
	PlayAnimation(ClickedAnim, FWidgetAssets::START_AT_BEGINNING, FWidgetAssets::SINGLE_PLAY_ANIMATION, EUMGSequencePlayMode::Forward, HalfAnimationSpeed, true);
}

void UAnimatedMenuButton::ActivateButton()
{
	UnbindAllFromAnimationFinished(ClickedAnim);
	const float HalfAnimationSpeed = AnimationSpeedMultiplier / 2.f;
	PlayAnimation(ClickedAnim, FWidgetAssets::START_AT_BEGINNING, FWidgetAssets::SINGLE_PLAY_ANIMATION, EUMGSequencePlayMode::Reverse, HalfAnimationSpeed);
	Super::ActivateButton();
}
