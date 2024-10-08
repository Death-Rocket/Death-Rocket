// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DRSpawnable.h"
#include "Explosion/DRExplosion.h"

#include "GameFramework/Actor.h"
#include "GeometryCollection/GeometryCollectionActor.h"

#include "DRExplosiveProp.generated.h"

UCLASS()
class DEATHROCKET_API ADRExplosiveProp : public ADRSpawnable // , public AGeometryCollectionActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	ADRExplosiveProp();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void Explode(FTransform transform);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explosion Type")
	TSubclassOf<ADRExplosion> BaseExplosion;

};
