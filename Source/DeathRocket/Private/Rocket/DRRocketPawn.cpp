// Fill out your copyright notice in the Description page of Project Settings.

//Game Related Classes
#include "Rocket/DRRocketPawn.h"
#include "Explosion/DRExplosion.h"
//Components
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/MeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/AudioComponent.h"
#include "NiagaraComponent.h"
//Libraries
#include "NiagaraFunctionLibrary.h"
#include "Engine/DamageEvents.h"


// Sets default values
ADRRocketPawn::ADRRocketPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SetupMesh();
	SetupCollision();
	SetupMovement();
	SetupSound();
	SetupParticle();
}

void ADRRocketPawn::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (bAlreadyCollided || OtherActor == nullptr || OtherActor == this || OtherActor == mCharacterOwner)
		return;
	if (OtherComp != nullptr)
	{
		bAlreadyCollided = true;
		//Stop Sound
		if (mLoopSound)
			mLoopSound->Stop();
		//Projection
		if (OtherComp->IsSimulatingPhysics())
			OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());
		if (OtherActor->IsA<ADRRocketPawn>())
		{
			Explode2Rockets(Hit.ImpactPoint);
			Cast<ADRRocketPawn>(OtherActor)->bAlreadyCollided = true;
			Cast<ADRRocketPawn>(OtherActor)->Destroy();
		}
		else
		{
			if (OtherActor->CanBeDamaged())
			{
				FPointDamageEvent DamageEvent(mDamage, Hit, GetActorForwardVector(), nullptr);
				OtherActor->TakeDamage(mDamage, DamageEvent, mCharacterOwner ? mCharacterOwner->Controller : nullptr, this);
			}
			Contact();
		}
	}
}

void ADRRocketPawn::SetTeamNumber(int Number)
{
	if (Number == -1)
		return;
	mTeamNb = Number;
	mMesh->SetMaterial(0, MaterialTeam[mTeamNb]);
}

int ADRRocketPawn::GetTeamNumber()
{
	return	mTeamNb;
}

void ADRRocketPawn::SetCharacterOwner(ADRCharacter* NewOwner)
{
	if (NewOwner == nullptr)
		return;
	this->mCharacterOwner = NewOwner;
	SetTeamNumber(mCharacterOwner->GetTeam());
}

float ADRRocketPawn::GetLength() const
{
	return mMesh->Bounds.BoxExtent.GetComponentForAxis(EAxis::X);
}

// Called when the game starts or when spawned
void ADRRocketPawn::BeginPlay()
{
	Super::BeginPlay();

	// Die after 5 seconds by default
	InitialLifeSpan = 40.0f;

	mWorldRef = GetWorld();
	if (mLoopSound)
		mLoopSound->Play();
	else
		UE_LOG(LogTemp, Warning, TEXT("Rocket %s: No Sound"), *this->GetName());
	SetTeamNumber(mTeamNb);
}
void ADRRocketPawn::SetupMesh()
{
	mMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MainMesh"));

	// Set as root component
	RootComponent = mMesh;

	mMesh->SetRelativeLocation({ 8.5,0.,0. });
}
void ADRRocketPawn::SetupCollision()
{
	mMesh->BodyInstance.SetCollisionProfileName("Projectile");
	mMesh->OnComponentHit.AddDynamic(this, &ADRRocketPawn::OnHit);		// set up a notification for when this component hits something blocking
	SetupCollisionChannels();
	// Players can't walk on it
	mMesh->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	mMesh->CanCharacterStepUpOn = ECB_No;
}

void ADRRocketPawn::SetupCollisionChannels()
{
	//Type Explosion
	mMesh->SetCollisionObjectType(ECC_GameTraceChannel2);
	//Channels responses
	//Default
	mMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);

	mMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	mMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Ignore);
	mMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel3, ECollisionResponse::ECR_Ignore);//Explosions ignore
	mMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel4, ECollisionResponse::ECR_Ignore);//MineTrigger ignore
}
void ADRRocketPawn::SetupParticle()
{
	Trace = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Trace"));
	Trace->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	Trace->SetRelativeLocation({ -56.5,0.,0. });
	Trace->SetRelativeRotation(FRotator::MakeFromEuler({ 0., 90., 0. }));
}
void ADRRocketPawn::Contact()
{
	Explode();
}
void ADRRocketPawn::Explode()
{
	//SecurityCheck
	if(mbIsExploding)
	 return;
	mbIsExploding = true;

	//Call Explosion
	if (BaseExplosion)
	{
		ADRExplosion* NewExplosion = mWorldRef->SpawnActor<ADRExplosion>(BaseExplosion.Get(), GetActorLocation(), GetActorRotation());
		NewExplosion->SetCharacterOwner(mCharacterOwner);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Rocket %s: No base explosion"), *this->GetName());
	}
	Destroy();
}
void ADRRocketPawn::Explode2Rockets(FVector ImpactPoint)
{

	if (BetweenRocketsExplosion)
	{
		ADRExplosion* NewExplosion = GetWorld()->SpawnActor<ADRExplosion>(BetweenRocketsExplosion.Get(), ImpactPoint, GetActorRotation());
		NewExplosion->SetCharacterOwner(nullptr);
	}
	else
		UE_LOG(LogTemp, Warning, TEXT("Rocket %s: No encounter explosion"), *this->GetName());
	Destroy();

}

void ADRRocketPawn::SetupSound()
{
	mLoopSound = CreateDefaultSubobject<UAudioComponent>(TEXT("LoopSound"));
	mLoopSound->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}
void ADRRocketPawn::SetupMovement()
{	// Use a ProjectileMovementComponent to govern this projectile's movement
	Movement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	Movement->UpdatedComponent = RootComponent;
	Movement->InitialSpeed = 3000.f;
	Movement->MaxSpeed = 3000.f;
	Movement->bRotationFollowsVelocity = true;
	Movement->bShouldBounce = true;
}

void ADRRocketPawn::StopLoopSound()
{
	mLoopSound->Stop();
}
// Called to bind functionality to input
void ADRRocketPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	APawn::SetupPlayerInputComponent(PlayerInputComponent);
}

