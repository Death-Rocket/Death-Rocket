// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DRControlZone.generated.h"

class ADRPlayerChar;
class ADR_GameState;

UCLASS()
class DEATHROCKET_API ADRControlZone : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADRControlZone();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	double CaptureTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	double PointInterval;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 PointAmount;

private:
	int32 m_CurrentTeamOwner;
	int32 m_CurrentCaptureTeam;
	double m_CaptureTimer;
	double m_PointTimer;
	bool m_Capturing;
	const int m_PointAddToUlt = 10;
	
	TArray<ADRPlayerChar*> m_PlayersList;

	ADR_GameState* m_GameStateRef;
	TArray<UMaterialInstanceDynamic*> m_PlanesMaterials;

	void UpdateCapture(float DeltaTime);
	void UpdatePoints(float DeltaTime);

	UFUNCTION()
	void EventBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION()
	void EventEndOverlap(AActor* OverlappedActor, AActor* OtherActor);
};
