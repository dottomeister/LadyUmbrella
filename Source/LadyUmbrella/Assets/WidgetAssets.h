// 
// WidgetAssets.h
// 
// Collection of pre-cached widget uassets.
// 
// Copyright Zulo Interactive. All Rights Reserved.
// 

#pragma once

#include "CoreMinimal.h"
#include "LadyUmbrella/UI/Basics/Input/InputElement.h"
#include "LadyUmbrella/UI/Basics/State/EPlatform.h"

namespace FWidgetAssets
{
	inline TSubclassOf<UInputElement> SliderElementTemplate;
	inline TSubclassOf<UInputElement> ToggleElementTemplate;
	inline TSubclassOf<UInputElement> SelectorElementTemplate;
	inline TSubclassOf<UUserWidget> SubsectionTitleTemplate;
	
	static TMap<EPlatform, FName> PlatformsKeys =
	{
		{EPlatform::PC, FName("KBM")},
		{EPlatform::XBOX, FName("XInputController")}
	};
	
	static const float START_AT_BEGINNING = 0.0f;
	static const int LOOP_ANIMATION = 0;
	static const int SINGLE_PLAY_ANIMATION = 1;
	static const FString EMPTY_LIST = "Empty";
	static const float STEP_GROWTH_RATE = 1.01f;
	
	static const float MINIMUM_SLIDER = 0.01f;
	static const float MAXIMUM_SLIDER = 1.f;

};
