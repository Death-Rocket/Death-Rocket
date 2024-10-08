// Fill out your copyright notice in the Description page of Project Settings.


#include "Spawn/DRExplosiveProp.h"
#include "Spawn/DRSpawnable.h"

// Sets default values
ADRExplosiveProp::ADRExplosiveProp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADRExplosiveProp::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADRExplosiveProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADRExplosiveProp::Explode(FTransform transform)
{
    if (BaseExplosion)
    {
        ADRExplosion* NewExplosion = GetWorld()->SpawnActor<ADRExplosion>(BaseExplosion.Get(), transform);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Barrel %s: No base explosion"), *this->GetName());
    }
    Destroy();
}

