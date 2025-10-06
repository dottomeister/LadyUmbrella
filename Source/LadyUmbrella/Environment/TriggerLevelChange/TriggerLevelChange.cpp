#include "TriggerLevelChange.h"

#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "EChapterNames.h"
#include "LadyUmbrella/Characters/Player/PlayerCharacter.h"
#include "LadyUmbrella/Util/SaveSystem/LU_GameInstance.h"
#include "LadyUmbrella/Weapons/Umbrella/Umbrella.h"

ATriggerLevelChange::ATriggerLevelChange()
{
	CollisionBoxComponent = CreateDefaultSubobject<UBoxComponent>("Collision Box Component");
	CollisionBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ATriggerLevelChange::OnBeginOverlap);
	
}

void ATriggerLevelChange::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                         UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// steps:
	/*
	 * First we set the last level completed to true.
	 * Then we find current level in progress map and we set it to the lastchapter unlocked and played, but not completed
	 */
	if (!IsValid(OtherActor) || !OtherActor->IsA(APlayerCharacter::StaticClass()))
	{
		return;
	}

	// Set the gameplay voices to the ChapterLevel
	const UWorld* World = GetWorld();
	ULU_GameInstance* GameInstance = FGlobalPointers::GameInstance;

	if (!IsValid(GameInstance) || !IsValid(World))
	{
		return;
	}
	ULU_SaveGame* SaveGameFile   = GameInstance->GetSaveGameFile();
	const FName CurrentLevelName = *UGameplayStatics::GetCurrentLevelName(World);
	
	if (!IsValid(SaveGameFile) || CurrentLevelName.IsEqual(""))
	{
		return;
	}
	
	if (FChaptersProgress* Value = SaveGameFile->GetChaptersProgress().Find(CurrentLevelName))
	{
		Value->bIsCompleted			= true;
		Value->bLastChapterUnlocked = false;
		Value->bLastChapterPlayed	= false;
		Value->bChapterUnlocked		= true;
	}

	/////////////////////
	//Aqui llamar a funcion de guardar datos actuales del umbrella
	////////////////////
	///
	GameInstance->SaveGame();
	
	const FString NameOfTheNextLevel = ToString(NextLevel);
	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(GameInstance, &ULU_GameInstance::PostLoadingLevel);

	GameInstance->OpenLevelWithLoad(NameOfTheNextLevel);
	//UGameplayStatics::OpenLevel(World, NameOfTheNextLevel);
}
