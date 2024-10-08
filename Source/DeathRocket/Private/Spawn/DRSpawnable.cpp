// Fill out your copyright notice in the Description page of Project Settings.


#include "Spawn/DRSpawnable.h"

// Sets default values
ADRSpawnable::ADRSpawnable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADRSpawnable::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADRSpawnable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool ADRSpawnable::IsDestroyed() const
{
	return Destroyed;
}

