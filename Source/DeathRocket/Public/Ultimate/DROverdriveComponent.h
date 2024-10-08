// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DRUltComponent.h"
#include "DROverdriveComponent.generated.h"

/**
 * 
 */
UCLASS()
class DEATHROCKET_API UDROverdriveComponent : public UDRUltComponent
{
	GENERATED_BODY()
	
	void BeginUltimate() override;
	void EndUltimate() override;

	UDROverdriveComponent();

	float mBaseReloadDuration;
};
