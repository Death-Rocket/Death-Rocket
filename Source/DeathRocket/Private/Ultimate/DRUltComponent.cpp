// Fill out your copyright notice in the Description page of Project Settings.


#include "Ultimate/DRUltComponent.h"
#include "Player/DRPlayerChar.h"

// Sets default values for this component's properties
UDRUltComponent::UDRUltComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UDRUltComponent::BeginPlay()
{
	Super::BeginPlay();
	mbActive = false;
	Owner = Cast<ADRPlayerChar>(this->GetOwner());
	mWorldTimerManagerRef= &this->GetOwner()->GetWorldTimerManager();
	// ...
	
}

void UDRUltComponent::CallUltimate()
{
	if (mbActive)
		return;
	BeginUltimate();
}

void UDRUltComponent::BeginUltimate()
{
	mbActive = true;
	FTimerHandle _;
	mWorldTimerManagerRef->SetTimer(_, this, &UDRUltComponent::EndUltimate, mUltDuration, false);
	Owner->SetActiveParticule(true);
}
void UDRUltComponent::EndUltimate()
{
	if (!mbActive)
		return;
	mbActive = false;

	
	Owner->GetPlayerController()->PlayerUltCharge = 0;
	Owner->GetPlayerController()->SetUltPercent(0);
	Owner->SetActiveParticule(false);

}