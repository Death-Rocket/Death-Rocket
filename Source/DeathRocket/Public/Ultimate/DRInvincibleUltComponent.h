// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DRUltComponent.h"
#include "DRInvincibleUltComponent.generated.h"

/**
 * 
 */
UCLASS()
class DEATHROCKET_API UDRInvincibleUltComponent : public UDRUltComponent
{
	GENERATED_BODY()
	
public:



protected:
	void BeginUltimate() override;
	void EndUltimate() override;
};
