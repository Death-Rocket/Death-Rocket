// Fill out your copyright notice in the Description page of Project Settings.


#include "Ultimate/DROverdriveComponent.h"
#include "Player/DRPlayerChar.h"

void UDROverdriveComponent::BeginUltimate()
{
	Super::BeginUltimate();
	Owner->bOverdrive = true;
	mBaseReloadDuration = Owner->GetReloadDuration();

	Owner->SetReloadDuration(mBaseReloadDuration/2.f);
}

void UDROverdriveComponent::EndUltimate()
{
	Super::EndUltimate();
	Owner->bOverdrive = false;
	Owner->SetReloadDuration(mBaseReloadDuration);
}

UDROverdriveComponent::UDROverdriveComponent()
{
	mUltDuration = 10.f;
}
