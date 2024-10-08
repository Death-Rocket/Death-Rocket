// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Explosion/DRExplosion.h"
#include "DRImplosion.generated.h"

class URadialForceComponent;
/**
 *
 */
UCLASS()
class DEATHROCKET_API ADRImplosion : public ADRExplosion
{
	GENERATED_BODY()

public:
	ADRImplosion();
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Explosion | Growth Duration ", meta = (AllowPrivateAccess = "true"))
	double mGrowthDuration = 1.;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Explosion | Pause Duration ", meta = (AllowPrivateAccess = "true"))
	double mPauseDuration = 2.;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Explosion | Shrink Duration ", meta = (AllowPrivateAccess = "true"))
	double mShrinkDuration = 2.;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explosion | Radial Force", meta = (AllowPrivateAccess = "true"))
	URadialForceComponent* mRadialForce;

	virtual void BeginPlay() override;

	virtual void ApplyForceOnStaticMesh(UStaticMeshComponent* StaticMesh, FVector Direction, double Force)override;
	virtual void ApplyForceOnCharacter(ADRCharacter* Character, FVector Direction, double Force)override;

	virtual void Tick(float DeltaTime)override;

	bool mbPaused = false;
	double mShrinkProgression = 0.;

};
