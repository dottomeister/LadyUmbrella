// 
// Crosshair.cpp
// 
// Implementation of the Crosshair class.
// 
// Copyright Zulo Interactive. All Rights Reserved.
// 


#include "Crosshair.h"
#include "Animation/WidgetAnimation.h"
#include "Components/Image.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Kismet/GameplayStatics.h"
#include "LadyUmbrella/Weapons/Umbrella/Umbrella.h"
#include "LadyUmbrella/Characters/GenericCharacter.h"
#include "LadyUmbrella/Components/HealthComponent/HealthComponent.h"

void UCrosshair::NativeConstruct()
{
	BaseCrosshairDotColor = CrosshairDot->GetColorAndOpacity();
	BaseCrosshairLeftBorder = CrosshairLeftBorder->GetColorAndOpacity();
	BaseCrosshairRightBorder = CrosshairRightBorder->GetColorAndOpacity();
	BaseHitMarker = HitMarker->GetColorAndOpacity();

	HitMarker->SetVisibility(ESlateVisibility::Hidden);
}

void UCrosshair::Show()
{
	FGlobalPointers::Umbrella->OnUmbrellaStateChanged.BindUObject(this, &UCrosshair::UpdateCrosshairState);
	FGlobalPointers::Umbrella->OnPelletHit.BindUObject(this, &UCrosshair::UpdateHitMarkers);
	FGlobalPointers::Umbrella->OnUmbrellaRangeAndSpread.BindUObject(this, &UCrosshair::UpdateCrosshairSize);
	FGlobalPointers::Umbrella->OnMaxPelletsPerShot.BindUObject(this, &UCrosshair::UpdateHitMarkersArray);
	FGlobalPointers::Umbrella->OnCanShoot.BindUObject(this, &UCrosshair::UpdateCrosshairAnimation);
	FGlobalPointers::Umbrella->OnShootingCadence.BindUObject(this, &UCrosshair::UpdateCrosshairCadence);
}

void UCrosshair::Hide()
{
	FGlobalPointers::Umbrella->OnUmbrellaStateChanged.Unbind();
	FGlobalPointers::Umbrella->OnPelletHit.Unbind();
	FGlobalPointers::Umbrella->OnUmbrellaRangeAndSpread.Unbind();
	FGlobalPointers::Umbrella->OnMaxPelletsPerShot.Unbind();
	FGlobalPointers::Umbrella->OnCanShoot.Unbind();
	FGlobalPointers::Umbrella->OnShootingCadence.Unbind();
	SetVisibility(ESlateVisibility::Hidden);
}

void UCrosshair::UpdateCrosshairState(const bool IsAiming, const bool IsShieldOpen, const bool IsOnEnemy, const bool IsOnHookable)
{
	if (IsAiming && !IsShieldOpen)
	{
		SetVisibility(ESlateVisibility::Visible);
		
		if (IsOnEnemy)
		{
			CrosshairDot->SetColorAndOpacity(OnEnemyCrosshairColor);
			CrosshairLeftBorder->SetColorAndOpacity(OnEnemyCrosshairColor);
			CrosshairRightBorder->SetColorAndOpacity(OnEnemyCrosshairColor);
			return;
		}

		if (IsOnHookable)
		{
			CrosshairDot->SetColorAndOpacity(FLinearColor::Blue);
			CrosshairLeftBorder->SetColorAndOpacity(FLinearColor::Blue);
			CrosshairRightBorder->SetColorAndOpacity(FLinearColor::Blue);
			return;
		}
		
		CrosshairDot->SetColorAndOpacity(BaseCrosshairDotColor);
		CrosshairLeftBorder->SetColorAndOpacity(BaseCrosshairLeftBorder);
		CrosshairRightBorder->SetColorAndOpacity(BaseCrosshairRightBorder);
	}
	else
	{
		SetVisibility(ESlateVisibility::Hidden);
	}
}

void UCrosshair::UpdateHitMarkers(const bool bHasHitGenericCharacter, TSet<AGenericCharacter*> EnemiesHitByPellets) const
{
	if (!bHasHitGenericCharacter)
	{
		for (UImage* HitMarkerCopy : HitMarkerArray)
		{
			if (HitMarkerCopy->GetVisibility() != ESlateVisibility::Visible)
			{
				break;
			}
			HitMarkerCopy->SetVisibility(ESlateVisibility::Hidden);
		}
		return;
	}
	for (int32 i = 0; i < EnemiesHitByPellets.Num(); ++i)
	{
		UImage* HitMarkerToShow = HitMarkerArray[i];
		AGenericCharacter* Enemy = EnemiesHitByPellets.Array()[i];
		HitMarkerToShow->SetVisibility(ESlateVisibility::Visible);
		HitMarkerToShow->SetColorAndOpacity(BaseHitMarker);
		
		if (Enemy->GetHealthComponent()->IsDead())
		{
			HitMarkerToShow->SetColorAndOpacity(HitMarkerKillColor);
		}
		FVector2D ViewportSize = FVector2D::ZeroVector;
		if (GEngine->GameViewport)
		{
			ViewportSize = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY());
		}
		const FVector2D PanelSize = CrosshairPanel->GetCachedGeometry().GetLocalSize();
		FVector2D ScreenPos;
		if (UGameplayStatics::ProjectWorldToScreen(GetOwningPlayer(), Enemy->GetHitPositions().Last(), ScreenPos))
		{
			const FVector2D LocalPos = (ScreenPos / ViewportSize) * PanelSize;
			if (UCanvasPanelSlot* ImageSlot = Cast<UCanvasPanelSlot>(HitMarkerToShow->Slot))
			{
				ImageSlot->SetPosition(LocalPos);
			}
		}
	}
}

void UCrosshair::UpdateCrosshairSize(const float BulletRange, const float SpreadAngleX, const float SpreadAngleZ) const
{
	const float OffsetX = FMath::Tan(FMath::DegreesToRadians(SpreadAngleX)) * BulletRange + 20.f;
	const float SizeY = ANGLE_TO_FULL_SPAN * FMath::Tan(FMath::DegreesToRadians(SpreadAngleZ)) * BulletRange;
	const float SizeX = SizeY;

	
	if (UCanvasPanelSlot* LeftSlot = Cast<UCanvasPanelSlot>(CrosshairLeftBorder->Slot))
	{
		LeftSlot->SetPosition(FVector2D(-OffsetX, CROSSHAIR_VERTICAL_CENTER));
		CrosshairLeftBorder->SetRenderScale(FVector2D(SizeX / 100,SizeY / 100));
	}
	
	if (UCanvasPanelSlot* RightSlot = Cast<UCanvasPanelSlot>(CrosshairRightBorder->Slot))
	{
		RightSlot->SetPosition(FVector2D(OffsetX, CROSSHAIR_VERTICAL_CENTER));
		CrosshairRightBorder->SetRenderScale(FVector2D(SizeX / 100,SizeY / 100));
	}
}

void UCrosshair::UpdateHitMarkersArray(const int32 MaxNrBulletPerShot)
{
	if (HitMarkerArray.Num() < MaxNrBulletPerShot)
	{
		const UCanvasPanelSlot* HitMarkerSlot = Cast<UCanvasPanelSlot>(HitMarker->Slot);
		for (int32 i = HitMarkerArray.Num(); i < MaxNrBulletPerShot; ++i)
		{
			UImage* HitMarkerCopy = DuplicateObject<UImage>(HitMarker, this);
			CrosshairPanel->AddChild(HitMarkerCopy);
			HitMarkerCopy->SetVisibility(ESlateVisibility::Hidden);
			UCanvasPanelSlot* HitMarkerCopySlot = Cast<UCanvasPanelSlot>(HitMarkerCopy->Slot);
			HitMarkerCopySlot->SetSize(HitMarkerSlot->GetSize()); 
			HitMarkerCopySlot->SetAlignment(HitMarkerSlot->GetAlignment());
			HitMarkerCopySlot->SetPosition(HitMarkerSlot->GetPosition());
			HitMarkerArray.Add(HitMarkerCopy);
		}
	}
}

void UCrosshair::UpdateCrosshairAnimation(const bool bIsShooting, const float ShootingCadence)
{
	if (!IsAnimationPlaying(ShootingCadenceAnim) && bIsShooting)
	{
		PlayAnimationReverse(ShootingCadenceAnim, ShootingCadence, false);
	}
	else
	{
		if (IsAnimationPlaying(ShootingCadenceAnim) && !bIsShooting)
		{
			StopAnimation(ShootingCadenceAnim);
		}
	}
}

void UCrosshair::UpdateCrosshairCadence(const FTimerHandle ShootingTimer)
{
	const float Elapsed = GetWorld()->GetTimerManager().GetTimerElapsed(ShootingTimer);
	const float Duration = GetWorld()->GetTimerManager().GetTimerRate(ShootingTimer);

	if (Duration <= 0.0f)
	{
		return;
	}
	
	const float Alpha = Elapsed / Duration;
	
	const float TargetTime = FMath::Lerp(
	ShootingCadenceAnim->GetEndTime(),
	ShootingCadenceAnim->GetStartTime(),
		Alpha
	);
	
	SetAnimationCurrentTime(ShootingCadenceAnim, TargetTime);
}


