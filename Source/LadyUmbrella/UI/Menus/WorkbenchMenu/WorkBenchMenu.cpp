// Fill out your copyright notice in the Description page of Project Settings.

#include "WorkBenchMenu.h"

#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Kismet/GameplayStatics.h"
#include "Components/ScrollBox.h"
#include "LadyUmbrella/Weapons/Umbrella/Umbrella.h"
#include "LadyUmbrella/Characters/Player/PlayerCharacter.h"
#include "LadyUmbrella/Components/UpgradeComponent/UpgradeComponent.h"
#include "LadyUmbrella/Controllers/MainController.h"
#include "LadyUmbrella/Interfaces/UmbrellaUpgradeInterface.h"
#include "LadyUmbrella/UI/Basics/DescriptionBlock.h"
#include "LadyUmbrella/UI/Menus/WorkbenchMenu/UpgradeSlot.h"
#include "LadyUmbrella/Util/LoggerUtil.h"
#include "LadyUmbrella/Weapons/Umbrella/UmbrellaLocation.h"

FOnWorkBenchExit UWorkBenchMenu::OnWorkBenchExit;

void UWorkBenchMenu::ShowCurrentDescription()
{
	if (!GetOptions().IsValidIndex(GetNavigationIndex()))
	{
		return;
	}
	UUpgradeSlot* CurrentElement = Cast<UUpgradeSlot>(GetOptions()[GetNavigationIndex()]);
	if (!IsValid(CurrentElement))
	{
		return;
	}
	DescriptionBlock->SetTitleText(FText::FromString(CurrentElement->GetText()));
	DescriptionBlock->SetDescriptionText(CurrentElement->GetDescription());
}

FReply UWorkBenchMenu::NativeOnPreviewKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	Super::NativeOnPreviewKeyDown(InGeometry, InKeyEvent);

	switch (IsNavigatingSomewhere(InKeyEvent.GetKey()))
	{
		case ENavigationInput::NAVIGATE_UP :
		{
			int32 PreviousIndex = GetNavigationIndex();
			MenuNavigation(false);
			if (PreviousIndex != GetNavigationIndex())
			{
				ShowCurrentDescription();
			}
			return FReply::Handled();
		}
		case ENavigationInput::NAVIGATE_DOWN :
		{
			int32 PreviousIndex = GetNavigationIndex();
			MenuNavigation(true);
			if (PreviousIndex != GetNavigationIndex())
			{
				ShowCurrentDescription();
			}
			return FReply::Handled();
		}
		case ENavigationInput::INTERACT :
			if (GetNavigationIndex() < 0)
			{
				return FReply::Handled();
			}
		
			if (UUpgradeSlot* CurrentElement = Cast<UUpgradeSlot>(GetOptions()[GetNavigationIndex()]))
			{
				CurrentElement->StartHold();
			}
			return FReply::Unhandled();
		
		case ENavigationInput::SWITCH_LEFT_TAB :
		case ENavigationInput::SWITCH_RIGHT_TAB :
			ToggleMenuOption();
			ShowCurrentDescription();
			return FReply::Handled();
		
		case ENavigationInput::EXIT :
			CloseMenu();
			return FReply::Handled();
	
		default:
			return FReply::Handled();
	}
}

FReply UWorkBenchMenu::NativeOnKeyUp(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	const bool bInteract = IsNavigatingSomewhere(InKeyEvent.GetKey()) == ENavigationInput::INTERACT;
	if (!bInteract)
	{
		return FReply::Handled();
	}

	if (GetNavigationIndex() >= 0 && GetNavigationIndex() < GetOptions().Num())
	{
		if (UUpgradeSlot* CurrentElement = Cast<UUpgradeSlot>(GetOptions()[GetNavigationIndex()]))
		{
			if (CurrentElement->IsPlayingAnimation())
			{
				CurrentElement->StopHold();
				return FReply::Handled();
			}
		}
	}
	
	return FReply::Handled();
}

void UWorkBenchMenu::UpdatePrices()
{
	if (!IsValid(FGlobalPointers::Umbrella))
	{
		return;
	}
	
	if (IUmbrellaUpgradeInterface* UpgradeComponentInterface = Cast<IUmbrellaUpgradeInterface>(FGlobalPointers::Umbrella))
	{
		if (!IsValid(UmbrellaUpgradeComponent))
		{
			UmbrellaUpgradeComponent = UpgradeComponentInterface->GetUmbrellaUpgradeComponent();
		}
		for (int32 Index = 0; Index < UpgradeSlotArray.Num(); Index++)
		{
			FUpgrade* Upgrade  = UmbrellaUpgradeComponent->GetUpgrades()->Find(static_cast<EUpgradeType>(Index));
			UpgradeSlotArray[Index]->UpdateLevel(*Upgrade);
		}
	}
}

void UWorkBenchMenu::SetUpInitialFocus()
{
	if (UPanelWidget* ActiveContainer = Cast<UPanelWidget>(Cast<UWidgetSwitcher>(GetContainer())->GetActiveWidget()))
	{
		GetButtonsFrom(ActiveContainer);
		GetOptions()[0]->SetFocus();
		GetOptions()[0]->SetKeyboardFocus();
		ShowCurrentDescription();
	}
	Shotgun->SetOpacity(1.f);
	Shield->SetOpacity(0.2f);
}

void UWorkBenchMenu::SetPiecesElement(const int32 NewPieces)
{
	// Leer de la variable de clase en lugar de por parametro
	FNumberFormattingOptions Options;
	Options.MinimumIntegralDigits = 3;
	AmountOfPlayerPieces->SetText(FText::AsNumber(NewPieces, &Options));
	SetPiecesOfThePlayer(NewPieces);
}

void UWorkBenchMenu::ReadUmbrellaUpgrades(const TMap<EUpgradeType, FUpgrade>& Upgrades)
{
	int32 ID = 0;

	UpgradeSlotArray.Empty();
	
	for (auto It = Upgrades.CreateConstIterator(); It; ++It, ++ID)
	{
		UUpgradeSlot* NewSlot = Cast<UUpgradeSlot>(CreateWidget(
			GetWorld(),
			UpgradeSlotClass,
			FName(It.Value().GetName().ToString() + FString(" Upgrade")))
		);

		if (IsValid(NewSlot))
        {
        	FUpgrade Upgrade = It.Value();
        	
        	NewSlot->SetupUpgradeSlots(Upgrade);
        	NewSlot->SetUpgradeId(ID);
        	NewSlot->UpdateLevel(Upgrade);
        	NewSlot->ProgressEndedDelegate.BindUObject(this, &UWorkBenchMenu::OnUpgradePressed);

        	UpgradeSlotArray.Add(NewSlot);
        }

		if (ID < Upgrades_Separator)
		{
			VB_Upgrades_Shield->AddChild(NewSlot);
		}
		else
		{
			VB_Upgrades_Shotgun->AddChild(NewSlot);
		}
	}
}

void UWorkBenchMenu::OnUpgradePressed(const int32 UpgradePressed)
{
	if (!IsValid(FGlobalPointers::Umbrella))
	{
		FLogger::ErrorLog("Invalid Global pointers in UWorkBenchMenu.OnUpgradePressed)");
		return;
	}
	
	if (!FGlobalPointers::Umbrella->Implements<UUmbrellaUpgradeInterface>() || !IsValid(FGlobalPointers::Umbrella->GetUpgradeComponent()))
	{
		FLogger::ErrorLog("Couldn't find Umbrella Upgrade Interface or Component in UWorkBenchMenu.OnUpgradePressed)");
		return;
	}

	const EUpgradeType Type = static_cast<EUpgradeType>(UpgradePressed);
	FUpgrade* Upgrade  = FGlobalPointers::Umbrella->GetUpgradeComponent()->GetUpgrades()->Find(Type);
	if (Upgrade->IsAvailable())
	{
		PiecesOfThePlayer -= Upgrade->GetPrice();
		Upgrade->SetLevel(Upgrade->GetLevel() + 1);
		FGlobalPointers::PlayerCharacter->SetCurrentPiecesForUpgrades(PiecesOfThePlayer);
		SetPiecesElement(PiecesOfThePlayer);
		UpdatePrices();
		UpgradeSlotArray[UpgradePressed]->UpdateLevel(*Upgrade);
		(void) FGlobalPointers::Umbrella->OnUmbrellaUpgradeDelegate.ExecuteIfBound(Type);
	}
}

void UWorkBenchMenu::CloseMenu()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>( UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (IsValid(PlayerController) && IsValid(PlayerCharacter))
	{
		PlayerController->SetInputMode(FInputModeGameOnly());
		PlayerController->SetShowMouseCursor(false);
		PlayerController->SetViewTargetWithBlend(PlayerCharacter, 0.5f, VTBlend_Linear, 0, false);
		SetVisibility(ESlateVisibility::Collapsed);
		OnWorkBenchExit.Broadcast(EUmbrellaLocation::Hand, true, false);
		OnWorkBenchExit.Broadcast(EUmbrellaLocation::Back, true, true);
	}
}

void UWorkBenchMenu::ToggleMenuOption()
{
	if (UWidgetSwitcher* Switcher = Cast<UWidgetSwitcher>(GetContainer()))
	{
		uint8 SwitchIndex = Switcher->GetActiveWidgetIndex() == 0 ? 1 : 0;
		if (SwitchIndex)
		{
			Switcher->SetActiveWidgetIndex(1);
			Shotgun->SetOpacity(0.2f);
			Shield->SetOpacity(1.f);
		}
		else
		{
			Switcher->SetActiveWidgetIndex(0);
			Shotgun->SetOpacity(1.f);
			Shield->SetOpacity(0.2f);
		}
		GetButtonsFrom(Cast<UPanelWidget>(Switcher->GetActiveWidget()));
		GetOptions()[0]->SetFocus();
		GetOptions()[0]->SetKeyboardFocus();
		SetNavigationIndex(0);
	}
}