// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryContext.h"
#include "EQC_TriggerVolume.generated.h"

/**
 * 
 */
UCLASS()
class LADYUMBRELLA_API UEQC_TriggerVolume : public UEnvQueryContext
{
	GENERATED_BODY()
	virtual void ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const override;

};
