// 
// AnimatedButton.h
// 
// Animated Button for UI. 
// 
// Copyright Zulo Interactive. All Rights Reserved.
//

#pragma once

#include "CoreMinimal.h"
#include "AnimatedBaseButton.h"
#include "Blueprint/UserWidget.h"
#include "AnimatedMenuButton.generated.h"

class UButton;
class UImage;
class UOverlay;

UCLASS()
class LADYUMBRELLA_API UAnimatedMenuButton : public UAnimatedBaseButton
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Animation")
	float AnimationSpeedMultiplier;

	UPROPERTY(BlueprintReadWrite, Category = "Animation", meta = (BindWidgetAnim, AllowPrivateAccess = "true"), Transient)
	UWidgetAnimation* ClickedAnim;

protected:
	
	virtual void GoToPressed() override; // Animation Handler
	virtual void ActivateButton() override; // Delegate Broadcast (or any additional logic)
};
