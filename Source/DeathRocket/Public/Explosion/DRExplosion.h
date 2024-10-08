// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "DRExplosion.generated.h"

UCLASS()
class DEATHROCKET_API ADRExplosion : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADRExplosion();


	/** called when Player caught in explosion*/
	UFUNCTION()
	virtual void OnOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit);
	/* Set owner and team at once*/
	UFUNCTION(Category = "Explosion | Owner")
	void SetCharacterOwner(ADRCharacter* NewOwner);
	UFUNCTION(Category = "Explosion | Owner")
	class ADRCharacter* GetCharacterOwner(ADRCharacter* NewOwner);
	UFUNCTION(Category = "Explosion | Team | Number")
	int GetTeamNumber();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Explosion | Danger Duration ", meta = (AllowPrivateAccess = "true", Tootltip = "Duration BEFORE TimeDilation"))
	double mDuration = 2.;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Explosion | Time Dilation ", meta = (AllowPrivateAccess = "true",ToolTip = "Affects all the timers"))
	double mTimeDilation = 1.;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Explosion | Damage ", meta = (AllowPrivateAccess = "true"))
	int mDamage = 1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Explosion | Collision ", meta = (AllowPrivateAccess = "true"))
	class USphereComponent* mDangerZone;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Explosion | Collision ",meta =( ClampMin = "0.", ClampMax = "1.", AllowPrivateAccess = "true"))
	double mRatioBeforeNoDamage = 0.6;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Explosion | Launch Force ", meta = (AllowPrivateAccess = "true"))
	double mLaunchForceIn = 1000.;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Explosion | Launch Force ", meta = (AllowPrivateAccess = "true"))
	double mLaunchForceOut = 300.;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explosion | Particles", meta = (AllowPrivateAccess = "true"))
	class UNiagaraComponent* mParticles;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Explosion | Sound", meta = (AllowPrivateAccess = "true"))
	UAudioComponent* mSound;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void SetupCollision();
	virtual void SetupCollisionChannels();
	virtual void SetupParticles();
	virtual void SetupSound();
	virtual bool IsBlocked(FVector ToActor);

	virtual void ApplyForceOnStaticMesh(UStaticMeshComponent* StaticMesh, FVector Direction, double Force);
	virtual void ApplyForceOnCharacter(ADRCharacter* Character, FVector Direction, double Force);

	UWorld* mWorldRef;
	ADRCharacter* mCharacterOwner = nullptr;
	float mMaxRadius;
	double mInvertedDuration;
	double mGrowthProgression;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
