// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DRSpawnable.h"
#include "GameFramework/Character.h"
#include "DRSpawner.generated.h"

UCLASS()
class DEATHROCKET_API ADRSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADRSpawner();

	UPROPERTY(EditAnywhere)
	double Frequency;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ADRSpawnable> ObjectToSpawn;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	double m_SpawnTimer;

	ADRSpawnable* m_SpawnedObject;

	void Spawn();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
