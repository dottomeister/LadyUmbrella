#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "SubsectionTitle.generated.h"

UCLASS()
class USubsectionTitle : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category = "Custom Button|Widgets", meta = (BindWidget, AllowPrivateAccess = "true"))
	UTextBlock* Title;

public:

	FText GetTitle() const { return Title->GetText(); }
	void SetTitle(const FText& Text) const { Title->SetText(Text); }
};
