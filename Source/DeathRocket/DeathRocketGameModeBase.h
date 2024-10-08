// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DeathRocketGameModeBase.generated.h"

/**
 * 
 */



UCLASS()
class DEATHROCKET_API ADeathRocketGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	ADeathRocketGameModeBase();


public :

	UFUNCTION(BlueprintCallable, Category = Team)
	FColor GetTeamColor(int TeamId) 
	{
		FColor color;
		switch (TeamId)
		{
		default:
			color = FColor::White;
			break;
		case 0 :
			color = FColor::Red;
			break;
		case 1:
			color = FColor::Blue;
			break;
		}

		return color;
	}

	UFUNCTION(BlueprintCallable, Category = Team)
	UMaterialInterface* GetOverlayMaterialColor(int TeamId)
	{
		if (TeamId + 1 >= OverlayMaterial.Num())
			return nullptr;

		// +1 for set overlay white for -1 team
		return OverlayMaterial[TeamId + 1 ];
	}

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Team)
	TArray<UMaterialInterface*> OverlayMaterial;




	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = Team)
	void OnGameBegin();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = Team)
	void OnGameEnd(int teamID);


	UFUNCTION(BlueprintCallable)
	void AllPlayerStopPlay();

};
// Copyright Epic Games, Inc. All Rights Reserved.



