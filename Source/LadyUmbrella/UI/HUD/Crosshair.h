// 
// Crosshair.h
// 
// Crosshair interface for the main player. 
// 
// Copyright Zulo Interactive. All Rights Reserved.
//


#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Crosshair.generated.h"

class UImage;
class UCanvasPanel;
class AGenericCharacter;

/**
 * 
 */

UCLASS()
class LADYUMBRELLA_API UCrosshair : public UUserWidget
{
	GENERATED_BODY()
	
	const float ANGLE_TO_FULL_SPAN = 2.0f;
	const float CROSSHAIR_VERTICAL_CENTER = 0.0f;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	UImage* CrosshairDot;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	UImage* CrosshairLeftBorder;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	UImage* CrosshairRightBorder;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	UImage* HitMarker;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	UCanvasPanel* CrosshairPanel;

	UPROPERTY(EditAnywhere, Category="On Enemy Color", meta = (AllowPrivateAccess = true))
	FLinearColor OnEnemyCrosshairColor;

	UPROPERTY(EditAnywhere, Category="On Enemy Death", meta = (AllowPrivateAccess = true))
	FLinearColor HitMarkerKillColor;

	UPROPERTY(BlueprintReadWrite, Category = "Animation", meta = (BindWidgetAnim, AllowPrivateAccess = "true"), Transient)
	UWidgetAnimation* ShootingCadenceAnim;

	UPROPERTY()
	TArray<UImage*> HitMarkerArray;
	
	FLinearColor BaseCrosshairDotColor;
	FLinearColor BaseCrosshairLeftBorder;
	FLinearColor BaseCrosshairRightBorder;
	FLinearColor BaseHitMarker;
	
public:
	
	UFUNCTION(BlueprintCallable, Category = Visibility)
	void Show();
	
	UFUNCTION(BlueprintCallable, Category = Visibility)
	void Hide();

	void UpdateCrosshairState(const bool IsAiming, const bool IsShieldOpen, const bool IsOnEnemy,const bool IsOnHookable);

	void UpdateHitMarkers(const bool bHasHitGenericCharacter, TSet<AGenericCharacter*> EnemiesHitByPellets) const;

	void UpdateCrosshairSize(const float BulletRange, const float SpreadAngleX, const float SpreadAngleZ) const;

	void UpdateHitMarkersArray(const int32 MaxNrBulletPerShot);

	void UpdateCrosshairAnimation(const bool bIsShooting, const float ShootingCadence);

	void UpdateCrosshairCadence(const FTimerHandle ShootingTimer);

protected:

	virtual void NativeConstruct() override;
	
};
