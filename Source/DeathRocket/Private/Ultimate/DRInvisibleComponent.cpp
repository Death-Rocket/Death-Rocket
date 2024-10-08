#include "Ultimate/DRInvisibleComponent.h"
#include "Player/DRPlayerChar.h"

void UDRInvisibleComponent::BeginUltimate()
{
	Super::BeginUltimate();
	Owner->SetInvisibilityOn(mOpacity);
	Owner->SetActiveParticule(false);
}

void UDRInvisibleComponent::EndUltimate()
{
	Super::EndUltimate();
	Owner->SetInvisibilityOff();
}
