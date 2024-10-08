// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/DRCharacter.h"
#include "Player/DRPlayerChar.h"
#include "Game/DRGameState.h"

#include "Kismet/BlueprintPlatformLibrary.h"
#include "DRGameInstance.generated.h"

class UDRUltComponent;


#define TEAMA 0 
#define TEAMB 1 
#define TEAMNULL -1

/**
 * 
 */
UCLASS()
class DEATHROCKET_API UDR_GameInstance : public UPlatformGameInstance
{
	GENERATED_BODY()

public :



	/**
	* @fn bool CheckIfTeamValid(TArray<ADRPlayerChar*> players)
	* @brief This function CheckIfThereIsa winnign Team
	* @return TheIdOfTheWinnerTeam if there is one else return -1.
	*/
	UFUNCTION(BlueprintCallable)
	bool CheckIfTeamValid(TArray<ADRPlayerChar*> players);

	/**
	* @fn  GetTeamArray();
	* @brief return The PlayersTeam
	*/
	UFUNCTION(BlueprintCallable)
	TArray<int> GetTeamArray() 
	{
		return mTeamArray;
	}

	/**
	* @fn TSubclassOf<UDRUltComponent> GetPlayerUltimate(int playerID)
	* @brief return The PlayersTeam
 	* @return the utlimate of th Player
	*/
	UFUNCTION(BlueprintCallable)
	TSubclassOf<UDRUltComponent> GetPlayerUltimate(int playerID);

	/**
	* @fn int GetPlayerGetTeam(int playerID)
	* @brief return The PlayersTeam
	*/
	UFUNCTION(BlueprintCallable)
	int GetPlayerGetTeam(int playerID)


	{
		if (playerID >= mMaxPlayer || playerID < 0)
			return TEAMNULL;

		if (mTeamArray.IsEmpty())
			return TEAMA;

		return mTeamArray[playerID];
	}
	/**
	* @fn void SetPlayerUltimate(int playerID,int ultId)
	* @brief SetThePlayer a ultimate ability
	*/
	UFUNCTION(BlueprintCallable)
	void SetPlayerUltimate(int playerID,int ultId)
	{
		if (mPlayerUltArray.Num() < GetNbrOfPlayer())
			mPlayerUltArray.SetNum(GetNbrOfPlayer());

		if (playerID >= mMaxPlayer || playerID < 0)
			return;

		if (ultId >= mUltArray.Num() || ultId < 0)
			return;

		mPlayerUltArray[playerID] = mUltArray[ultId];
	}

	/**
	* @fn int GetNbrOfPlayer() 
	* @brief The nbrOfPlayer 2/4
	*/
	UFUNCTION(BlueprintCallable)
	int GetNbrOfPlayer() 
	{
		return mTeamArray.Num();
	}

	/**
	* @fn void SetNbrOfPlayer(int number) 
	* @brief will reserve the correct amout of data for the players
	*/
	UFUNCTION(BlueprintCallable)
	void SetNbrOfPlayer(int number) 
	{
		mTeamArray.SetNum(number);

		for (size_t i = 0; i < mTeamArray.Num(); i++)
		{
			mTeamArray[i] = TEAMNULL;
		}

		mPlayerUltArray.SetNum(number);
	}
	/**
	* @fn CreateDebugTeam()
	* @brief Create a team for debuf purpose
	*/
	UFUNCTION(BlueprintCallable)
	void CreateDebugTeam();
	/**
	* @fn void CreateDebugUltPlayer()
	* @brief Create BaseUltForMaxPlayer[DEBUGONLY]
	*/
	UFUNCTION(BlueprintCallable)
	void CreateDebugUltPlayer();

	/**
	* @fn IsPlayerSelectUltValid()
	* @brief Check If all player have select a ultimate ability
	*/
	UFUNCTION(BlueprintCallable)
	bool IsPlayerSelectUltValid()
	{

		for (size_t i = 0; i < mPlayerUltArray.Num(); i++)
		{
			if(mPlayerUltArray[i] == nullptr)
			{
				return false;
			}
		}

		return true;
	}
	/**
	* @fn void ResetGameData()
	* @brief ResetGameData
	*/
	UFUNCTION(BlueprintCallable)
	void ResetGameData()
	{
		mPlayerUltArray.Empty();
		mTeamArray.Empty();

	}



	

protected:

	
	const int mMaxPlayer{ 4 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = " GameStatus ")
	TArray<int> mTeamArray;

	// SavedUltPlayer
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = " GameStatus ")
	TArray<TSubclassOf<UDRUltComponent>> mPlayerUltArray;

	// Array of SelectableUlt
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = " GameStatus ")
	TArray<TSubclassOf<UDRUltComponent>> mUltArray;


};
