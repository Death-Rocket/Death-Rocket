// Fill out your copyright notice in the Description page of Project Settings.


#include <Explosion/DRImplosion.h>
#include <Character/DRCharacter.h>

#include <Components/SphereComponent.h>
#include <PhysicsEngine/RadialForceComponent.h>


ADRImplosion::ADRImplosion()
{
	//We want this one to be slower
	mTimeDilation = 1.;
	mLaunchForceIn = mLaunchForceOut = -3000000.; //Excessive for black hole effect
	mRadialForce = CreateDefaultSubobject<URadialForceComponent>(TEXT("Attraction"));
	mRadialForce->Radius = mDangerZone->GetUnscaledSphereRadius();
	mRadialForce->ForceStrength = mLaunchForceIn;
	mRadialForce->bIgnoreOwningActor = true;
	mRadialForce->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}
void ADRImplosion::BeginPlay()
{
	Super::BeginPlay();
	mRadialForce->Radius = 0.01f;
}
void ADRImplosion::ApplyForceOnStaticMesh(UStaticMeshComponent* StaticMesh, FVector Direction, double Force)
{
	//Radial Force takes care of this now
}
void ADRImplosion::ApplyForceOnCharacter(ADRCharacter* Character, FVector Direction, double Force)
{
	//Radial Force takes care of this now
}

// Called every frame
void ADRImplosion::Tick(float DeltaTime)
{
	DeltaTime *= mTimeDilation;
	AActor::Tick(DeltaTime);
	if (mGrowthProgression > mDuration)
	{
		mDangerZone->SetSphereRadius (0.);//Deactivate
		mRadialForce->Radius = 0.;
		mRadialForce->ForceStrength = 0.;
		return;
	}
	//Init Growing Part
	if (mGrowthProgression == 0.)
		mInvertedDuration = 1. / mGrowthDuration;
	mGrowthProgression += DeltaTime;
	//Growing Part
	if (mGrowthProgression < mGrowthDuration)
	{
		//Here to embbigen the radius over time
		double NewRadius = mMaxRadius * mGrowthProgression * mInvertedDuration;
		mDangerZone->SetSphereRadius(NewRadius);
		mRadialForce->Radius = NewRadius;
	}
	//Pausing Part(Only Init)
	else if (mGrowthProgression < mGrowthDuration + mPauseDuration)
	{
		if (!mbPaused)
		{
		mbPaused = true; //flag will stay on, to not use after this phase
		mDangerZone->SetSphereRadius(mMaxRadius);
		mRadialForce->Radius = mMaxRadius;
		mInvertedDuration = 1. / mShrinkDuration; //Init Shrink once
		}
	}
	//Shrink Part
	else
	{
		mShrinkProgression += DeltaTime;
		//Here to shrink the radius over time
		double NewRadius = mMaxRadius * (1 - mShrinkProgression * mInvertedDuration);
		mDangerZone->SetSphereRadius(NewRadius);
		mRadialForce->Radius = NewRadius;
	}
}