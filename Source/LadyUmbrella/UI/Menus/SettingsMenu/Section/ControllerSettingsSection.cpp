// Fill out your copyright notice in the Description page of Project Settings.

#include "ControllerSettingsSection.h"

#include "LadyUmbrella/UI/Basics/Input/InputType/SelectorInput.h"
#include "LadyUmbrella/UI/Menus/PauseMenu/PauseMenu.h"
#include "LadyUmbrella/Util/LoggerUtil.h"

void UControllerSettingsSection::NativeConstruct()
{
	Super::NativeConstruct();
	/*
	USelectorInput* TempSelector = GetSelector("Resolution");
	TempSelector->OnOptionUpdated.AddDynamic(this, &UVideoSettingsSection::UpdateResolution);
	TempSelector->SetCurrentOptionIndex(0);
	TempSelector->UpdateOptions();
	*/
}

void UControllerSettingsSection::UpdateResolution(uint8 Index)
{
	FLogger::InfoLog("[TODO] Update Resolution Delegate");
}
