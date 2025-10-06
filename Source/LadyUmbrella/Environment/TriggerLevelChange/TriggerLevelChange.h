#pragma once
#include "CoreMinimal.h"
#include "TriggerLevelChange.generated.h"

enum class EChapterNames : uint8;
class UBoxComponent;

UCLASS()
class LADYUMBRELLA_API ATriggerLevelChange : public AActor
{
	
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision Box", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* CollisionBoxComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level to Open", meta = (AllowPrivateAccess = "true"))
	EChapterNames NextLevel;

public:
	ATriggerLevelChange();
	
	UFUNCTION(BlueprintCallable)	
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
