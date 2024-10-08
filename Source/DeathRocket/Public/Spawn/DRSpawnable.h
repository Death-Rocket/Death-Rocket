// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <functional>
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DRSpawnable.generated.h"

UCLASS()
class DEATHROCKET_API ADRSpawnable : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADRSpawnable();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Destroyed;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	bool IsDestroyed() const;
};
