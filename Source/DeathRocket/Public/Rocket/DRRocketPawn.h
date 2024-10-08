// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <vector>

#include "CoreMinimal.h"
#include "../Character/DRCharacter.h"

#include "Materials/Material.h"
#include "NiagaraComponent.h"
#include "GameFramework/Pawn.h"

#include "DRRocketPawn.generated.h"

class UBoxComponent;
class UProjectileMovementComponent;
class ADRExplosion;

UCLASS()
class DEATHROCKET_API ADRRocketPawn : public APawn
{
	GENERATED_BODY()

	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sound", meta = (AllowPrivateAccess = "true"))
	UAudioComponent* mLoopSound;

public:
	// Sets default values for this pawn's properties
	ADRRocketPawn();

	/** called when projectile hits something  Should be overridden for mines*/
	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	UFUNCTION(Category = "Rocket | Team | Number")
	void SetTeamNumber(int Number);
	UFUNCTION(BlueprintCallable, Category = "Rocket | Team | Number")
	int GetTeamNumber();

	UFUNCTION(BlueprintCallable, Category = "Rocket | Owner")
	void SetCharacterOwner(ADRCharacter* NewOwner);
	UFUNCTION(BlueprintCallable, Category = "Rocket | Owner")
	ADRCharacter* GetCharacterOwner() { return mCharacterOwner; };

	/* Returns ProjectileMovement subobject */
	UProjectileMovementComponent* GetMovement() const { return Movement; }
	/*To be called from a collider*/
	virtual void Explode();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explosion Type")
	TSubclassOf<ADRExplosion>  BaseExplosion;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explosion Type")
	TSubclassOf<ADRExplosion> BetweenRocketsExplosion;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particles | Trace")
	UNiagaraComponent* Trace;
	
	float GetLength() const;

	bool bAlreadyCollided = false;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* mMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* Movement;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DirectHit | Damage")
	float mDamage = 2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Team | Material")
	TArray<UMaterialInterface*> MaterialTeam;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Team")
	int mTeamNb = 0;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void SetupMesh();
	virtual void SetupSound();
	virtual void SetupMovement() ;
	virtual void SetupCollision();
	virtual void SetupCollisionChannels();
	virtual void SetupParticle();
	virtual void Contact();
	virtual void Explode2Rockets(FVector ImpactPoint);
	void StopLoopSound();

	ADRCharacter* mCharacterOwner;

	UWorld* mWorldRef;

	bool mbIsExploding = false;
public:

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
