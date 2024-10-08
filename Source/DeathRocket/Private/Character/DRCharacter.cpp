// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/DRCharacter.h"
#include "Rocket/DRRocketPawn.h"

#include "Components/CapsuleComponent.h"
#include "Components/MeshComponent.h"
#include "Components/AudioComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "GameFramework/GameModeBase.h"
#include "Animation/AnimInstance.h"

// Sets default values
ADRCharacter::ADRCharacter()
{
	SetupSound();
	mWorldRef = GetWorld();
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate
	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	mTeam = -1;

	
}

// Called when the game starts or when spawned
void ADRCharacter::BeginPlay()
{
	Super::BeginPlay();
	mHealth = mMaxHealth;
	mCurrentAmmo = mMaxAmmo;
	mActiveRocket = mBaseRocket;
	mWorldTimerManagerRef = &GetWorldTimerManager();
	mBaseParticle = GetComponentByClass<UParticleSystemComponent>();
	SetActiveParticule(false);
	
}
float ADRCharacter::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (bInvincible || !this->IsAlive() || mbBeingDamaged) //To avoid double damage
		return 0.f;
	if (bOverdrive)
	{
		if (EventInstigator != GetController())
		{
			mHealth -= 1;
		}
		else
		{
			return 0.f;
		}
	}
	else
	{
		mHealth -= Damage;
	}
	mbBeingDamaged = true;
	OnDamage();

	if (!this->IsAlive())
	{
		if (EventInstigator && EventInstigator != this->Controller && EventInstigator->GetCharacter()->IsA<ADRCharacter>())
		{
			//Call the OnKill Event
			Cast<ADRCharacter>(EventInstigator->GetCharacter())->OnKill();
		}
		this->Die(); //Function
		this->OnDie(); //Event
	}
	else
	{
		bInvincible = true;
		FTimerHandle _;
		mWorldTimerManagerRef->SetTimer(_, this, &ADRCharacter::StopGrace, mGraceDuration, false);
		mWorldTimerManagerRef->SetTimer(mBlinkTimerHandle, this, &ADRCharacter::Blink, mVisibilityOffDuration, true);

	}
	mbBeingDamaged = false;
	return Damage;
}
void ADRCharacter::Die()
{
	GetCharacterMovement()->DisableMovement();
	GetMovementComponent()->StopMovementImmediately();
	GetCapsuleComponent()->SetSimulatePhysics(true);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);//Explosions ignore

	GetMesh()->AnimScriptInstance->StopAllMontages(0.1f);
	//Here for Death animation
	if (mDeathAnimSequence)
	{
		float MontageDuration = mDeathAnimSequence->GetPlayLength();
		GetMesh()->PlayAnimation(mDeathAnimSequence, false);

		FTimerHandle _;
		float TimeAfterDeathHide = 2.f;
		float TimeAfterDeathDestroy = 2.1f;

		mWorldTimerManagerRef->SetTimer(_, this, &ADRCharacter::HidePlayerMesh, MontageDuration + TimeAfterDeathHide, false);
		SetLifeSpan(MontageDuration + TimeAfterDeathDestroy);
	}
	else
	{
		float TimeAfterDeathDestroy = 4.1f;
		UE_LOG(LogTemp, Warning, TEXT("Character - Death Montage is missing"));
		HidePlayerMesh();
		SetLifeSpan(TimeAfterDeathDestroy);
	}
}

void ADRCharacter::StopGrace()
{
	bInvincible = false;
	mWorldTimerManagerRef->ClearTimer(mBlinkTimerHandle);
	this->GetMesh()->SetVisibility(true, true);
}
void ADRCharacter::Blink()
{
	this->GetMesh()->SetVisibility(!GetMesh()->IsVisible(), true);
}
void ADRCharacter::HidePlayerMesh()
{
	GetMesh()->SetActive(false);
	GetMesh()->SetVisibility(false, true);
}

// Called to bind functionality to input
void ADRCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ADRCharacter::SetTeam(AGameModeBase* gameMode, int32 teamId)
{

	mTeam = teamId;
}

int32 ADRCharacter::GetTeam()
{
	return mTeam;
}

int ADRCharacter::GetCurrentAmmo()
{
	return mCurrentAmmo;
}

int ADRCharacter::GetMaxAmmo()
{
	return mMaxAmmo;
}
void ADRCharacter::ShootRocket(FVector Location, FRotator Rotation)
{
	//Already reloading
	if (mbReloading)
		return;
	//Out of ammo
	if (!mCurrentAmmo)
	{
		if (mDryFireSound)
			mDryFireSound->Play();
		return;
	}
	//PlaySound
	if (mShootSound)
		mShootSound->Play();
	else
		UE_LOG(LogTemp, Warning, TEXT("Character - Shoot Sound is missing"));
	const FTransform StartTransform = FTransform{ Rotation, Location };
	//Security, alway Reset to BaseRocket
	if (!mActiveRocket)
		mActiveRocket = mBaseRocket;
	ADRRocketPawn* NewRocket = mWorldRef->SpawnActorDeferred<ADRRocketPawn>(mActiveRocket, StartTransform, this, this, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
	if (NewRocket)
	{
		NewRocket->SetCharacterOwner(this);
		NewRocket->FinishSpawning(StartTransform);

		FVector launchVector = Rotation.RotateVector(-FVector::ForwardVector) * mRecoilPower;

		// add a little bit of launch power in the air
		launchVector.Z += 0.5f * mRecoilPower;
		LaunchCharacter(launchVector, true, true);
	}

	//Call event
	OnShoot();
	//Reset Rocket
	mActiveRocket = mBaseRocket;
	if (!mbNoReload)
	{
		//Call event
		OnSpendAmmo();
		mCurrentAmmo--;
		if (!mCurrentAmmo)
			//This condition here or conflict with the Landed event
			if (this->GetMovementComponent()->IsMovingOnGround())
				Reload();
	}

}
void ADRCharacter::Reload()
{
	if (mbNoReload)
	{
		StopReload();
	}
	else
	{
		FTimerHandle _;
		FTimerHandle __;

		mbReloading = true;
		/** When Immobile, reload will be 200% faster */
		float ActualReloadDuration = mReloadDuration;
		float MinimumDelay = 0.2f; //0.2f is arbitrary for sound clarity
		if (IsImmobile())
			ActualReloadDuration = mReloadDuration * mImmobileReloadRatio;
		mWorldTimerManagerRef->SetTimer(__, this, &ADRCharacter::StopReload, ActualReloadDuration);
		mWorldTimerManagerRef->SetTimer(_, this, &ADRCharacter::PlayReloadSound, std::max(ActualReloadDuration - mReloadSound->GetSound()->GetDuration(), MinimumDelay));

		//Double check better than none
		if (mReloadAnimMontage)
			mWorldTimerManagerRef->SetTimer(_, this, &ADRCharacter::OnReload, std::max(ActualReloadDuration - mReloadAnimMontage->GetPlayLength(), MinimumDelay)); //MinimumDelay to be synchroned
	}
}
bool ADRCharacter::SetActiveRocket(TSubclassOf<ADRRocketPawn> NewActiveRocket)
{
	mActiveRocket = NewActiveRocket;
	return (mActiveRocket.GetDefaultObject() != nullptr);
}
void ADRCharacter::PlayReloadSound()
{
	if (mReloadSound)
		mReloadSound->Play();
}

bool ADRCharacter::IsImmobile()
{
	return GetMovementComponent()->Velocity.SquaredLength() < mImmobileThreshold * mImmobileThreshold;
}
void ADRCharacter::StopReload()
{
	mbReloading = false;
	mCurrentAmmo = mMaxAmmo;
	OnReloaded();
}
void ADRCharacter::SetupSound()
{
	mSoundlist = CreateDefaultSubobject<USceneComponent>(TEXT("Soundlist"));
	mDryFireSound = CreateDefaultSubobject<UAudioComponent>(TEXT("DryFireSound"));
	mDryFireSound->AttachToComponent(mSoundlist, FAttachmentTransformRules::KeepRelativeTransform);
	mDryFireSound->bAutoActivate = false;
	mShootSound = CreateDefaultSubobject<UAudioComponent>(TEXT("ShootSound"));
	mShootSound->AttachToComponent(mSoundlist, FAttachmentTransformRules::KeepRelativeTransform);
	mShootSound->bAutoActivate = false;
	mReloadSound = CreateDefaultSubobject<UAudioComponent>(TEXT("ReloadSound"));
	mReloadSound->AttachToComponent(mSoundlist, FAttachmentTransformRules::KeepRelativeTransform);
	mReloadSound->bAutoActivate = false;
}