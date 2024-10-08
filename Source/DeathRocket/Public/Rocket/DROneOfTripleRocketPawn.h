// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DRRocketPawn.h"
#include "DROneOfTripleRocketPawn.generated.h"

/**
 * 
 */
UCLASS()
class DEATHROCKET_API ADR_OneOfTripleRocketPawn : public ADRRocketPawn
{
	GENERATED_BODY()
	ADRRocketPawn* m_OriginalRocket;
public:
	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;
	UFUNCTION(BlueprintCallable)
	void SetOriginalRocket(ADRRocketPawn* Original);
	ADRRocketPawn* GetOriginalRocket() { return m_OriginalRocket; }
};
