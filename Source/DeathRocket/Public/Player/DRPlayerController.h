// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "../Buff/DRBuff.h"
#include "DRPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class DEATHROCKET_API ADR_PlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	ADR_PlayerController();

	UFUNCTION(BlueprintImplementableEvent)
	void SetUltPercent(float percent);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Variable)
	float PlayerUltCharge = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Variable)
	int MaxBuffNbr = 3;
	 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Variable)
	float UltPercentBonusOnKill = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Variable)
	TArray<ADR_Buff*> buffs;

	UFUNCTION(BlueprintCallable)
	bool GetBuff(ADR_Buff* newBuff,int& buffIndex);

	/*
	* @fn bool HasReachMaxBuff()
	* CheckIfThere a empty Space
	*/
	UFUNCTION(BlueprintCallable)
	bool HasReachMaxBuff();

protected: 
	UFUNCTION(BlueprintCallable)
	void UseBuff(int buffIndex);
	


};
