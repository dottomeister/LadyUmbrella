// Fill out your copyright notice in the Description page of Project Settings.


#include "UpgradePieces.h"

#include "Components/StaticMeshComponent.h"
#include "LadyUmbrella/UI/Basics/Icon/InteractiveIcon.h"

AUpgradePieces::AUpgradePieces()
{
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	StaticMeshComponent->SetupAttachment(RootComponent);

	ConstructorHelpers::FClassFinder<UInteractiveIcon> WidgetFinder(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Widgets/WidgetBlueprints/Components/WBP_InteractiveIcons.WBP_InteractiveIcons_C'"));

	if (WidgetFinder.Succeeded())
	{
		SetWidgetClass(WidgetFinder.Class);
	}
	
	SetAmountToPickup(1);

	TypeOfPickable = true;
}

void AUpgradePieces::BeginPlay()
{
	Super::BeginPlay();
}



int32 AUpgradePieces::Interacting()
{
	GetPickUpSoundComponent()->SetParameter(PickUpEventParameterName, TypeOfPickable);
	
	GetPickUpSoundComponent()->PlayEvent();

	SetActorHiddenInGame(true);

	GetWorldTimerManager().SetTimer(PickUpTimerHandle, this, &AGenericPickable::DestroyInteractable, 2.f, false);
	
	//Destroy();
	
	return GetAmountToPickup();
}

int32 AUpgradePieces::Interacting(APlayerCharacter* Interactor)
{
	Super::Interacting(Interactor);
	
	return 0;
}