// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DRRocketPawn.h"
#include "DRMine.generated.h"

class USphereComponent;

/**
 * 
 */
UCLASS()
class DEATHROCKET_API ADRMine : public ADRRocketPawn
{
	GENERATED_BODY()
public:
	ADRMine();

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit);


protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision | TriggerZone ")
	USphereComponent* mTriggerZone;
	virtual void BeginPlay() override;

	void SetupTriggerZone();
	void FirstTriggerCheck();

	virtual float TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	//Event
	UFUNCTION(BlueprintImplementableEvent, Category = "First Collision")
	void OnContact();
	virtual void Contact() override;
	bool mbFirstContact = true;
};
