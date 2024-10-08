// Fill out your copyright notice in the Description page of Project Settings.


#include "Explosion/DRExplosion.h"
#include <Character/DRCharacter.h>

#include <Components/AudioComponent.h>
#include <Components/SphereComponent.h>
#include "NiagaraComponent.h"

#include <NiagaraSystem.h>
#include <NiagaraFunctionLibrary.h>
#include <Kismet/KismetMathLibrary.h>
#include "Engine/DamageEvents.h"

ADRExplosion::ADRExplosion()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SetCanBeDamaged(false);
	SetupCollision();
	SetupSound();
	SetupParticles();
}

void ADRExplosion::OnOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
	if (OtherActor == this || OtherActor == nullptr)
		return;

	FVector TargetLoc = OtherActor->GetActorLocation();
	if (IsBlocked(TargetLoc))
		return;
	if (!OtherComp->IsA<UStaticMeshComponent>() && !OtherActor->IsA<ADRCharacter>())
		return;


	double Distance = FVector::Distance(this->GetActorLocation(), TargetLoc);
	FVector LaunchVector = (TargetLoc - this->GetActorLocation()) / Distance; //Normalized
	double LaunchForce = 0.;
	ADRCharacter* Character = nullptr;
	if (mMaxRadius * mRatioBeforeNoDamage > Distance)
	{
		LaunchForce = mLaunchForceIn;

		if (OtherActor->CanBeDamaged())
		{
			Character = Cast<ADRCharacter>(OtherActor);
			if (Character &&										//It is a character
				Character != mCharacterOwner &&						//Different from Explosion Owner
				mCharacterOwner &&									//Character owner is defined
				Character->GetTeam() == mCharacterOwner->GetTeam())	//Same team as owner
			{
				//No Damage
			}
			else
			{
				//Another way to do this is to use a RadialDamageEvent
				FPointDamageEvent DamageEvent(mDamage, Hit, GetActorForwardVector(), nullptr);
				OtherActor->TakeDamage(mDamage, DamageEvent, mCharacterOwner ? mCharacterOwner->Controller : nullptr, this);
			}
		}
	}
	else
	{
		LaunchForce = mLaunchForceOut;
	}
	UStaticMeshComponent* StaticMesh = Cast<UStaticMeshComponent>(OtherComp);
	if (StaticMesh && StaticMesh->IsSimulatingPhysics())
	{
		ApplyForceOnStaticMesh(StaticMesh, LaunchVector, LaunchForce * -mWorldRef->GetGravityZ() * 0.1f * StaticMesh->GetMass());
	}
	else
	{
		//Could have adoided cast earlier
		if (!Character)
			Character = Cast<ADRCharacter>(OtherActor);
		if (Character)
		{
			ApplyForceOnCharacter(Character, LaunchVector, LaunchForce);
		}
	}
}

void ADRExplosion::SetCharacterOwner(ADRCharacter* NewOwner)
{
	this->mCharacterOwner = NewOwner;
}

ADRCharacter* ADRExplosion::GetCharacterOwner(ADRCharacter* NewOwner)
{
	return mCharacterOwner;
}

int ADRExplosion::GetTeamNumber()
{
	return mCharacterOwner->GetTeam();
}

void ADRExplosion::ApplyForceOnStaticMesh(UStaticMeshComponent* StaticMesh, FVector Direction, double Force)
{
	StaticMesh->AddForce(Direction * Force);
}
void ADRExplosion::ApplyForceOnCharacter(ADRCharacter* Character, FVector Direction, double Force)
{
	Character->LaunchCharacter(Direction * Force, true, true);
}

bool ADRExplosion::IsBlocked(FVector ToActor)
{
	FVector ExplosionLocation = this->GetActorLocation();

	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	// Ignore self
	CollisionParams.AddIgnoredActor(this);

	// Perform the line trace
	bool bHit = GetWorld()->LineTraceSingleByObjectType(
		HitResult,
		ExplosionLocation,
		ToActor,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_WorldStatic), // Adjust the collision channel as needed
		CollisionParams
	);

	return bHit;
}

// Called when the game starts or when spawned
void ADRExplosion::BeginPlay()
{
	mParticles->SetRandomSeedOffset(UKismetMathLibrary::RandomInteger(int32(-1)));
	Super::BeginPlay();
	mWorldRef = GetWorld();
	SetLifeSpan(mDuration/mTimeDilation + 0.1f);//Apply TimeDilation + 0.1 for safety
	mMaxRadius = mDangerZone->GetUnscaledSphereRadius();
	mGrowthProgression = 0.;
	mInvertedDuration = 1. / mDuration;
	mParticles->SetCustomTimeDilation(mTimeDilation);


	if (mSound)
		mSound->Play();
	else
		UE_LOG(LogTemp, Warning, TEXT("Explosion %s: No Sound"), *this->GetName());
	//Here to start with almost no radius then grow over time
	mDangerZone->SetSphereRadius(0.01);

}

void ADRExplosion::SetupCollision()
{
	// Use a Box as a simple collision representation
	mDangerZone = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	RootComponent = mDangerZone;
	mDangerZone->SetMobility(EComponentMobility::Stationary);
	mDangerZone->InitSphereRadius(465.0f);
	mDangerZone->BodyInstance.SetCollisionProfileName("Explosion");

	SetupCollisionChannels();

	mDangerZone->OnComponentBeginOverlap.AddDynamic(this, &ADRExplosion::OnOverlap);		// set up a notification for when this component hits something blocking

	// Players can't walk on it
	mDangerZone->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	mDangerZone->CanCharacterStepUpOn = ECB_No;
}
void ADRExplosion::SetupCollisionChannels()
{
	//Type Explosion
	mDangerZone->SetCollisionObjectType(ECC_GameTraceChannel3);
	//Channels responses
		//Default
	mDangerZone->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	mDangerZone->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	mDangerZone->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Ignore);
	mDangerZone->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel2, ECollisionResponse::ECR_Ignore);//Rocket ignore
	mDangerZone->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel3, ECollisionResponse::ECR_Ignore);//Explosions ignore
}

void ADRExplosion::SetupParticles()
{
	mParticles = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Particles"));
	mParticles->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	mParticles->SetRelativeLocation({ 0. ,0.,1. });//To avoid collision with ground
}

void ADRExplosion::SetupSound()
{
	mSound = CreateDefaultSubobject<UAudioComponent>(TEXT("Sound"));
	mSound->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

// Called every frame
void ADRExplosion::Tick(float DeltaTime)
{
	DeltaTime *= mTimeDilation;
	AActor::Tick(DeltaTime);
	if (mGrowthProgression > mDuration)
	{
		mDangerZone->SetSphereRadius(0.);//Deactivate
		return;
	}
	mGrowthProgression += DeltaTime;
	//Here to embbigen the radius over time
	mDangerZone->SetSphereRadius( mMaxRadius * mGrowthProgression * mInvertedDuration);
}

