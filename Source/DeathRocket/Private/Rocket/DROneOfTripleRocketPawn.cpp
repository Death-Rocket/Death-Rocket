// Fill out your copyright notice in the Description page of Project Settings.


#include "Rocket/DROneOfTripleRocketPawn.h"

void ADR_OneOfTripleRocketPawn::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	//Ignore the two others
	if (OtherActor->IsA<ADR_OneOfTripleRocketPawn>() && Cast<ADR_OneOfTripleRocketPawn>(OtherActor)->GetOriginalRocket() == m_OriginalRocket)
		return;
	ADRRocketPawn::OnHit(HitComp, OtherActor, OtherComp, NormalImpulse, Hit);
}

void ADR_OneOfTripleRocketPawn::SetOriginalRocket(ADRRocketPawn* Original)
{
	m_OriginalRocket = Original;
	mTeamNb = m_OriginalRocket->GetTeamNumber();
}
