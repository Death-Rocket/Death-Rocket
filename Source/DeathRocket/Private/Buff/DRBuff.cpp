// Fill out your copyright notice in the Description page of Project Settings.

#include "Buff/DRBuff.h"
#include "Character/DRCharacter.h"
#include "Player/DRPlayerController.h"


// Sets default values
ADR_Buff::ADR_Buff()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void ADR_Buff::OnPickupByPlayer(ADRCharacter* playerCharacter, ADR_PlayerController* playerController)
{
	OnPickup(playerCharacter, playerController);
}

void ADR_Buff::OnConsumeByPlayer(ADRCharacter* playerCharacter, ADR_PlayerController* playerController)
{
	OnConsume(playerCharacter, playerController);
}

// Called when the game starts or when spawned
void ADR_Buff::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADR_Buff::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

