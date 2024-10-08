// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DRUltComponent.h"
#include "DRInvisibleComponent.generated.h"

/**
 * 
 */
UCLASS()
class DEATHROCKET_API UDRInvisibleComponent : public UDRUltComponent
{
	GENERATED_BODY()
	
public:

	UDRInvisibleComponent()
	{
		mUltDuration = 5.f;
	}

protected:


	void BeginUltimate() override;
	void EndUltimate() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Opacity")
	float mOpacity = 0.05f;
};