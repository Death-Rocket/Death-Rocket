// Fill out your copyright notice in the Description page of Project Settings.


#include "Rocket/DRMine.h"
//Components
#include "GameFramework/ProjectileMovementComponent.h"
#include <Components/SphereComponent.h>
#include "Components/BoxComponent.h"


ADRMine::ADRMine()
{
	SetupTriggerZone();
}

void ADRMine::BeginPlay()
{
	Super::BeginPlay();
}
void ADRMine::SetupTriggerZone()
{
	// Use a Box as a simple collision representation
	mTriggerZone = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	mTriggerZone->bAutoActivate = false;
	mTriggerZone->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	mTriggerZone->InitSphereRadius(300.f);
	mTriggerZone->BodyInstance.SetCollisionProfileName("TriggerZone");
	//Type MineTrigger
	mTriggerZone->SetCollisionObjectType(ECC_GameTraceChannel4);
	//Channels responses
	//Default
	mTriggerZone->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);

	mTriggerZone->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	mTriggerZone->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Ignore);
	mTriggerZone->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel2, ECollisionResponse::ECR_Ignore);//Rocket ignore
	mTriggerZone->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel4, ECollisionResponse::ECR_Ignore);//MineTrigger ignore

	// Players can't walk on it
	mTriggerZone->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	mTriggerZone->CanCharacterStepUpOn = ECB_No;

}

void ADRMine::Contact()
{
	if (!mbFirstContact)
	{
		Explode();
		return;
	}
	//Reset Flag
	mbFirstContact = false;
	//Set it "immortal"
	SetLifeSpan(0.f);
	//Stop everything
	Movement->StopMovementImmediately();
	Movement->Deactivate();
	Trace->Deactivate();
	StopLoopSound();
	FirstTriggerCheck();
	//Here to set active at last minute (SetActive isn't reliable)
	mTriggerZone->OnComponentBeginOverlap.AddDynamic(this, &ADRMine::OnBeginOverlap);
	//Sensible to explosions again
	mMesh->SetCollisionObjectType(ECC_GameTraceChannel1); //Pawn no camera collision
	mMesh->SetCollisionResponseToChannel(ECC_GameTraceChannel3, ECR_Overlap);

	OnContact();
	SetCanBeDamaged(true);
}

void ADRMine::OnBeginOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
	if (OtherActor == this || OtherActor == nullptr)
		return;

	if (OtherComp->IsA<UStaticMeshComponent>() || OtherActor->IsA<ADRCharacter>())
		Explode();
}
void ADRMine::FirstTriggerCheck()
{
	TArray<AActor*> ActorsInside;
	mTriggerZone->GetOverlappingActors(ActorsInside, ADRCharacter::StaticClass());
	if (!ActorsInside.IsEmpty() && !ActorsInside.Contains(this->mCharacterOwner))
		Explode();
}

float ADRMine::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if(Damage && !mbIsExploding)
		Explode();
	return Damage;
}
