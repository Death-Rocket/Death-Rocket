#include "Player/DRPlayerController.h"
#include "Player/DRPlayerChar.h"

#define BASEROCKETRESET 0

ADR_PlayerController::ADR_PlayerController()
{
	buffs.SetNum(MaxBuffNbr);
}

bool ADR_PlayerController::GetBuff(ADR_Buff* newBuff, int& buffIndex)
{
	for (size_t i = 0; i < buffs.Num(); i++)
	{
		if(buffs[i] == nullptr)
		{
			ADR_Buff*& ptr = buffs[i];
			ptr = newBuff;
			buffIndex = i;
			return true;
		}
	}
	return false;
}

bool ADR_PlayerController::HasReachMaxBuff()
{

	for (size_t i = 0; i < buffs.Num(); i++)
	{
		if (buffs[i] == nullptr)
		{
			return false;
		}
	}

	return true;
}

void ADR_PlayerController::UseBuff(int buffIndex)
{

	if (!buffs.IsValidIndex(buffIndex))
		return;

	if (buffs[buffIndex] == nullptr)
		return;

	ADRCharacter* playerCharacter = Cast<ADRCharacter>(GetPawn());

	if (playerCharacter == nullptr)
		return;


	buffs[buffIndex]->OnConsumeByPlayer(playerCharacter, this);

	
	if (buffIndex == BASEROCKETRESET)
		return;


	buffs[buffIndex]->Destroy();
	buffs[buffIndex] = nullptr;
}
