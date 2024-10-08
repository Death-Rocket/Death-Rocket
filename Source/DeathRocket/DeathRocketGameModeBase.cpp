// Copyright Epic Games, Inc. All Rights Reserved.


#include "DeathRocketGameModeBase.h"
#include <Kismet/GameplayStatics.h>

ADeathRocketGameModeBase::ADeathRocketGameModeBase()
{
}

void ADeathRocketGameModeBase::AllPlayerStopPlay()
{
	TArray<AActor*> PlayerController;


	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerController::StaticClass(), PlayerController);

	for (size_t i = 0; i < PlayerController.Num(); i++)
	{
		APlayerController* ptr = Cast<APlayerController>(PlayerController[i]);
		ptr->UnPossess();
	}


}
