// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/DRGameInstance.h"

#include <GameFramework/GameUserSettings.h>
#include <Kismet/GameplayStatics.h>




bool UDR_GameInstance::CheckIfTeamValid(TArray<ADRPlayerChar*> players)
{
	AGameModeBase* gamemode = GetWorld()->GetAuthGameMode();

	if (!gamemode)
		return false;

	

	int teamA = 0;
	int teamB = 0;

	// Team are alwayOfSameSize
	int TotalPlayerPerTeam = GetNbrOfPlayer() / 2;

	for (int i = 0 ; i != players.Num();i++)
	{
		if (players[i]->GetTeam() == TEAMA)
			teamA++;

		if (players[i]->GetTeam() == TEAMB)
			teamB++;

		

	}

	if (teamA == TotalPlayerPerTeam && teamB == TotalPlayerPerTeam)
	{
		mTeamArray.Reset();
		for (int i = 0; i != players.Num(); i++)
		{
			mTeamArray.Add(players[i]->GetTeam());
		}
		return true;
	}
	return false;
}



void UDR_GameInstance::CreateDebugTeam()
{
	mTeamArray.SetNum(mMaxPlayer);

	for (size_t i = 0; i < mTeamArray.Num(); i++)
	{
		mTeamArray[i] = i % 2;
	}
}

TSubclassOf<UDRUltComponent> UDR_GameInstance::GetPlayerUltimate(int playerID)
{
	if (playerID >= mMaxPlayer || playerID < 0)
		return mUltArray[3]; // rambo one is more funnier tbh

	if (mPlayerUltArray.IsEmpty() || !mPlayerUltArray.IsValidIndex(playerID))
		return mUltArray[3];

	return mPlayerUltArray[playerID];
}

void UDR_GameInstance::CreateDebugUltPlayer()
{

	mPlayerUltArray.SetNum(mMaxPlayer);
	
	for (size_t i = 0; i < mPlayerUltArray.Num(); i++)
	{
		mPlayerUltArray[i] = mUltArray[i];
	}
}
