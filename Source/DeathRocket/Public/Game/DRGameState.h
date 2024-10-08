// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Character/DRCharacter.h"
#include "DRGameState.generated.h"

class APlayerRespawnPoint;
class ADRCharacter;
class ADRPlayerChar;

#define MAXTEAM 2 

/**
 * 
 */
UCLASS()
class DEATHROCKET_API ADR_GameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly)
	TArray<APlayerRespawnPoint*> PlayerRespwanPoints;

	UFUNCTION(BlueprintCallable)
	const FTransform& GetRespawnPoint(int teamId);

	/**
	* @fn void PopGameModeOverlay()
	* @brief ShouldBeTriggerAtBegin
	*/
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = Team)
	void PopGameModeOverlay();

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 PointToWin = 100;
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<int32> TeamPoint;

	/**
	* @fn void OnTeamPointUpdate()
	* @brief ShouldBeCallWhenATeamEarnePoint
	*/
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = Team)
	void OnTeamPointUpdate();

	/**
	* @fn void AddPointToTeam(int32 teamToAddPoint,int nbrOfPointToAdd)
	* @brief AddPoitToTheTeam
	*/
	UFUNCTION(BlueprintCallable)
	void AddPointToTeam(int32 teamToAddPoint,int nbrOfPointToAdd);
	
	/**
	* @fn int CheckWinningTeam()
	* @brief will return the team index if winnig team return -1 if there is no winning team and will return Winner team if it is
	*/ 
	UFUNCTION(BlueprintCallable)
	int CheckWinningTeam();

	/**
	* @fn int SetInvincibility(bool value)
	* @brief Will PutToogleForAllPlayer invicibility
	*/
	UFUNCTION(BlueprintCallable)
	void SetInvincibility(bool value);

	/**
	* @fn TArray<ADRPlayerChar*> GetAllPlayer()
	* @brief GetAllPlayerInTheScene
	*/
	UFUNCTION(BlueprintCallable)
	TArray<ADRPlayerChar*> GetAllPlayer();

	/**
	* @fn void GetAllRespawnPoint();
	* @brief WillGetAllThePlayerRespawn In the current map
	*/
	void GetAllRespawnPoint();

};
