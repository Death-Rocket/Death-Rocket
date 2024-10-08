// Fill out your copyright notice in the Description page of Project Settings.

#include "Terrain/DRControlZone.h"
#include <string>
#include "Game/DRGameState.h"
#include "Player/DRPlayerChar.h"

// Sets default values
ADRControlZone::ADRControlZone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADRControlZone::BeginPlay()
{
	Super::BeginPlay();

	OnActorBeginOverlap.AddDynamic(this, &ADRControlZone::EventBeginOverlap);
	OnActorEndOverlap.AddDynamic(this, &ADRControlZone::EventEndOverlap);

	m_GameStateRef = GetWorld()->GetGameState<ADR_GameState>();

	m_CurrentTeamOwner = -1;

	TArray<UObject*> components;
	GetDefaultSubobjects(components);

	for (UObject* obj : components)
	{
		if (obj->GetName().Contains("Plane"))
		{
			UMeshComponent* mesh = Cast<UMeshComponent>(obj);
			UMaterialInstanceDynamic* mat = UMaterialInstanceDynamic::Create(mesh->GetMaterial(0), obj);
			mesh->SetMaterial(0, mat);

			m_PlanesMaterials.Add(mat);
		}
	}
}

// Called every frame
void ADRControlZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateCapture(DeltaTime);
	UpdatePoints(DeltaTime);
}

void ADRControlZone::UpdateCapture(float DeltaTime)
{
	if (!m_Capturing)
		return;

	m_CaptureTimer -= DeltaTime;

	if (m_CaptureTimer <= 0)
	{
		FVector4 color;
		if (m_CurrentCaptureTeam == 0)
			color = FVector4(1, 0, 0, 1);
		else
			color = FVector4(0, 0, 1, 1);

		
		for (UMaterialInstanceDynamic* mat : m_PlanesMaterials)
		{
			mat->SetVectorParameterValue("TeamColor", color);
		}

		m_CurrentTeamOwner = m_CurrentCaptureTeam;
		m_Capturing = false;

		m_PointTimer = PointInterval;
	}
}

void ADRControlZone::UpdatePoints(float DeltaTime)
{
	if (m_Capturing)
	{
		// Currently capturing, don't update points
		return;
	}

	if (m_CurrentTeamOwner == -1)
	{
		// No one owns this zone
		return;
	}

	m_PointTimer -= DeltaTime;

	if (m_PointTimer <= 0)
	{
		if (m_GameStateRef)
			m_GameStateRef->AddPointToTeam(m_CurrentTeamOwner, PointAmount);

		for (ADRPlayerChar* player : m_PlayersList)
		{
			if (player->GetTeam() == m_CurrentTeamOwner)
			{
				ADR_PlayerController* controller = player->GetPlayerController();
				controller->PlayerUltCharge += m_PointAddToUlt;
				controller->SetUltPercent(controller->PlayerUltCharge/100.f);
			}
		}

		m_PointTimer = PointInterval;
	}
}

void ADRControlZone::EventBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	if (OtherActor == nullptr)
		return;

	if (!OtherActor->IsA<ADRPlayerChar>())
		return;

	ADRPlayerChar* player = Cast<ADRPlayerChar>(OtherActor);
	int32 team = player->GetTeam();

	if (m_PlayersList.Num() == 0 && m_CurrentTeamOwner != team)
	{
		// Start capture
		m_CaptureTimer = CaptureTime;
		m_Capturing = true;
		m_CurrentCaptureTeam = team;
	}

	m_PlayersList.Add(player);
}

void ADRControlZone::EventEndOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	if (OtherActor == nullptr)
		return;

	if (!OtherActor->IsA<ADRPlayerChar>())
		return;

	ADRPlayerChar* player = Cast<ADRPlayerChar>(OtherActor);

	m_PlayersList.Remove(player);

	if (m_PlayersList.Num() == 0)
	{
		m_Capturing = false;
	}
}
