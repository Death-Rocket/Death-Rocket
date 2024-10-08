// Fill out your copyright notice in the Description page of Project Settings.


#include "Ultimate/DRInvincibleUltComponent.h"
#include "Player/DRPlayerChar.h"
#include "Kismet/GameplayStatics.h"

void UDRInvincibleUltComponent::BeginUltimate()
{
	Super::BeginUltimate();
	Owner->bInvincible = true;
}
void UDRInvincibleUltComponent::EndUltimate()
{
	Super::EndUltimate();
	Owner->bInvincible = false;
}

