// Fill out your copyright notice in the Description page of Project Settings.


#include "Ultimate/DRNoReloadComponent.h"
#include "Player/DRPlayerChar.h"

void UDRNoReloadComponent::BeginUltimate()
{
	Super::BeginUltimate();
	Owner->SetbNoReload(true);
}

void UDRNoReloadComponent::EndUltimate()
{
	Super::EndUltimate();
	Owner->SetbNoReload(false);
}
