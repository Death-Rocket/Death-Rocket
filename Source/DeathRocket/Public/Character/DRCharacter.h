// Fill out your copyright notice in the Description page of Project invttings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Particles/ParticleSystemComponent.h"
#include "DRCharacter.generated.h"

class USkeletalMeshComponent;

class UAnimMontage;
class USoundBase;
class AGameModeBase;
class ADRRocketPawn;

UCLASS()
class DEATHROCKET_API ADRCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADRCharacter();
	
	UFUNCTION(BlueprintCallable)
	void SetTeam(AGameModeBase* gameMode, int32 teamId);
	UFUNCTION(BlueprintCallable)
	int32 GetTeam();
	UFUNCTION(BlueprintCallable)
	int GetCurrentAmmo();
	UFUNCTION(BlueprintCallable)
	int GetMaxAmmo();

	UFUNCTION(BlueprintCallable, Category = "Character | Rocket | Active")
	bool SetActiveRocket(TSubclassOf <ADRRocketPawn> NewActiveRocket);
	void PlayReloadSound();

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Variable)
	bool bInvincible;

	bool bOverdrive;


	bool IsAlive() { return mHealth > 0; }
	void SetbNoReload(bool Value) { mbNoReload = Value; }
	void SetReloadDuration(float Value) { mReloadDuration = Value; }
	float GetReloadDuration() { return mReloadDuration; }

	void SetActiveParticule(bool Value) { mBaseParticle->SetActive(Value); };

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character | Animations | Death", DisplayName = "Death Sequence", meta = (AllowPrivateAccess = "true"))
	UAnimSequence* mDeathAnimSequence; 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character | Animations | Death", DisplayName = "Reload Montage", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* mReloadAnimMontage;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "Character | Sound", meta = (AllowPrivateAccess = "true"))
	UAudioComponent* mShootSound;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "Character | Sound", meta = (AllowPrivateAccess = "true"))
	UAudioComponent* mReloadSound;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "Character | Sound", meta = (AllowPrivateAccess = "true"))
	UAudioComponent* mDryFireSound;

	void StopReload();
	bool mbNoReload = false;
	bool mbBeingDamaged = false;

	USceneComponent* mSoundlist;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Variable)
	bool mbInvisible;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Rocket, meta = (AllowPrivateAccess = "true"))
	TSubclassOf <ADRRocketPawn> mBaseRocket;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Rocket, meta = (AllowPrivateAccess = "true"))
	TSubclassOf <ADRRocketPawn> mActiveRocket;

	//Health part
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Variable)
	int mHealth;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Variable)
	int mMaxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Variable)
	float mVisibilityOffDuration = 0.1f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Variable)
	float mGraceDuration = 0.5f;
	FTimerHandle mBlinkTimerHandle;
	/** When Immobile (speed under this value), reload will be faster */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Variable)
	double mImmobileThreshold = 10.;
	/** When Immobile, reload will be that time faster */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Variable)
	double mImmobileReloadRatio = 0.5;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Variable)
	float mReloadDuration = 1.5f;

	UParticleSystemComponent* mBaseParticle;

	bool IsImmobile();
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void SetupSound();

	virtual float TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual void Die();
	//Events
	UFUNCTION(BlueprintImplementableEvent, Category = "Character | Death")
	void OnDie();
	UFUNCTION(BlueprintImplementableEvent, Category = "Character | Damage")
	void OnDamage();
	UFUNCTION(BlueprintImplementableEvent, Category = "Character | Kill")
	void OnKill();
	UFUNCTION(BlueprintImplementableEvent, Category = "Character | Shoot")
	void OnShoot();
	UFUNCTION(BlueprintImplementableEvent, Category = "Character | Shoot")
	void OnSpendAmmo();
	UFUNCTION(BlueprintImplementableEvent, Category = "Character | Reload")
	void OnReload();
	UFUNCTION(BlueprintImplementableEvent, Category = "Character | Reload")
	void OnReloaded();


	int mCurrentAmmo = 0;
	int mMaxAmmo = 2;

	UFUNCTION(BlueprintCallable, Category = Rocket, meta = (AllowPrivateAccess = "true"))
	void ShootRocket(FVector Location, FRotator Rotation);
	void Reload();

	virtual void StopGrace();

	virtual void Blink();

	virtual void HidePlayerMesh();

	UWorld* mWorldRef;
	FTimerManager* mWorldTimerManagerRef;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Variable)
	int mTeam = -1;

	bool mbReloading = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Variable, meta = (AllowPrivateAccess = "true"))
	float mRecoilPower = 350.f;

};
