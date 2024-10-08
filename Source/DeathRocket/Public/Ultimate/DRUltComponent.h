// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DRUltComponent.generated.h"

class ADRPlayerChar;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DEATHROCKET_API UDRUltComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDRUltComponent();

	virtual void CallUltimate();


protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void BeginUltimate();
	virtual void EndUltimate();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float mUltDuration = 2.5f;

	bool mbActive;
	FTimerManager* mWorldTimerManagerRef;

public:	

	ADRPlayerChar* Owner;
		
};
