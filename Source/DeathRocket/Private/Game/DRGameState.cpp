// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/DRGameState.h"
#include "Terrain/DRPlayerRespawnPoint.h"
#include "Character/DRCharacter.h"
#include "Player/DRPlayerChar.h"
#include "../DeathRocketGameModeBase.h"

#include <string>
#include <Kismet/GameplayStatics.h>

void ADR_GameState::BeginPlay()
{
	Super::BeginPlay();
	GetAllRespawnPoint();
	

	TeamPoint.SetNum(MAXTEAM);
}

const FTransform& ADR_GameState::GetRespawnPoint(int teamId)
{
	TArray<int> respawnPointIndex;

	if (teamId == -1)
		teamId = 0;

	// Make sure that the array is not empty
	if (PlayerRespwanPoints.IsEmpty())
		GetAllRespawnPoint();


	for (size_t i = 0; i < PlayerRespwanPoints.Num(); i++)
	{
		if(teamId == PlayerRespwanPoints[i]->TeamID)
		{
			respawnPointIndex.Add(i);
		}
	}

	int randomIndex = FMath::RandRange(0, respawnPointIndex.Num());


	APlayerRespawnPoint* respawnPoint = PlayerRespwanPoints[randomIndex];

	return respawnPoint->GetTransform();
}

void ADR_GameState::AddPointToTeam(int32 teamToAddPoint, int nbrOfPointToAdd)
{
	
	TeamPoint[teamToAddPoint] += nbrOfPointToAdd;
	OnTeamPointUpdate();

	int CheckTeamWing = -1;
	CheckTeamWing = CheckWinningTeam();

	if(CheckTeamWing != -1)
	{
		ADeathRocketGameModeBase* gamemode = Cast<ADeathRocketGameModeBase>(GetWorld()->GetAuthGameMode());
		gamemode->OnGameEnd(CheckTeamWing);
	}

}

int ADR_GameState::CheckWinningTeam()
{

	if (TeamPoint.IsEmpty())
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::White, "Team point are empty  ");
		return -1;
	}

	for (size_t i = 0; i < TeamPoint.Num(); i++)
	{
		if(TeamPoint[i] >= PointToWin) 
		{
			return i;
		}
	}

	return -1;
}

void ADR_GameState::SetInvincibility(bool value)
{
	TArray<ADRPlayerChar*> players = GetAllPlayer();;

	for (size_t i = 0; i < players.Num(); i++)
	{
		players[i]->bInvincible = value;
	}

}

TArray<ADRPlayerChar*> ADR_GameState::GetAllPlayer()
{
	TArray<AActor*> AActorplayer;
	TArray<ADRPlayerChar*> playersArray;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADRPlayerChar::StaticClass(), AActorplayer);
	playersArray.Reserve(AActorplayer.Num());


	for (int i = 0; i < AActorplayer.Num(); i++)
	{
		playersArray.Add(Cast<ADRPlayerChar>(AActorplayer[i]));
	}

	return playersArray;
}

void ADR_GameState::GetAllRespawnPoint()
{
	TArray<AActor*> AActorplayerRespwanPoints;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerRespawnPoint::StaticClass(), AActorplayerRespwanPoints);
	PlayerRespwanPoints.Reserve(AActorplayerRespwanPoints.Num());

	for (int i = 0; i < AActorplayerRespwanPoints.Num(); i++)
	{
		PlayerRespwanPoints.Add(Cast<APlayerRespawnPoint>(AActorplayerRespwanPoints[i]));
	}
}


