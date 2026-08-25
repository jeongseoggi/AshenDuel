// Fill out your copyright notice in the Description page of Project Settings.


#include "GCN_HitEffect.h"

#include "NiagaraFunctionLibrary.h"

UGCN_HitEffect::UGCN_HitEffect()
{
}

bool UGCN_HitEffect::OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const
{
	if (HitNiagaraEffect)
	{
		FVector SpawnLocation = Parameters.Location;
		FRotator SpawnRotation = Parameters.Normal.Rotation();
		
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			MyTarget ? MyTarget->GetWorld() : nullptr,
			HitNiagaraEffect,
			SpawnLocation,
			SpawnRotation
		);
	}

	return true;
}

