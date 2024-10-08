// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DRBuff.generated.h"

class ADRCharacter;
class ADR_PlayerController;

UCLASS()
class DEATHROCKET_API ADR_Buff : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADR_Buff();


	UFUNCTION(BlueprintImplementableEvent)
	void OnPickup(ADRCharacter* playerCharacter, ADR_PlayerController* playerControlle);

	UFUNCTION(BlueprintImplementableEvent)
	void OnConsume(ADRCharacter* playerCharacter, ADR_PlayerController* playerController);



	UFUNCTION(BlueprintCallable)
	void OnPickupByPlayer(ADRCharacter* playerCharacter, ADR_PlayerController* playerControlle);

	UFUNCTION(BlueprintCallable)
	void OnConsumeByPlayer(ADRCharacter* playerCharacter, ADR_PlayerController* playerControlle);


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Variable)
	UTexture2D* BuffImage;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
