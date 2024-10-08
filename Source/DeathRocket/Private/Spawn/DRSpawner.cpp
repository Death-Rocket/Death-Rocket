// Fill out your copyright notice in the Description page of Project Settings.


#include "Spawn/DRSpawner.h"
#include "Spawn/DRSpawnable.h"
#include <string>

// Sets default values
ADRSpawner::ADRSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ADRSpawner::BeginPlay()
{
	Super::BeginPlay();

	m_SpawnTimer = Frequency;
}

void ADRSpawner::Spawn()
{
	FVector pos = GetActorLocation();
	FActorSpawnParameters spawnParams;

	spawnParams.Template = Cast<AActor>(ObjectToSpawn.Get());
	m_SpawnedObject = Cast<ADRSpawnable>(GetWorld()->SpawnActor(ObjectToSpawn.Get(), &pos, nullptr, spawnParams));
}


// Called every frame
void ADRSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (m_SpawnTimer == -1)
	{
		// Waiting for the signal that the spawn object was destroyed
		if (m_SpawnedObject->IsDestroyed())
		{
			m_SpawnTimer = Frequency;
			m_SpawnedObject = nullptr;
		}

		return;
	}

	m_SpawnTimer -= DeltaTime;

	if (m_SpawnTimer <= 0)
	{
		Spawn();
		m_SpawnTimer = -1;
	}
}

