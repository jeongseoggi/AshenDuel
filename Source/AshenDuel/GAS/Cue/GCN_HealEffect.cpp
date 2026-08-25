// Fill out your copyright notice in the Description page of Project Settings.


#include "GCN_HealEffect.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "GameFramework/Character.h"


AGCN_HealEffect::AGCN_HealEffect()
{
	PrimaryActorTick.bCanEverTick = false;
	
	AttachSocketName = NAME_None;
}

bool AGCN_HealEffect::OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters)
{
	Super::OnExecute_Implementation(MyTarget, Parameters);
	
	if (!MyTarget || !HealEffectSystem)
	{
		return false;
	}
	
	USceneComponent* AttachTargetComp = nullptr;
	
	if (ACharacter* TargetCharacter = Cast<ACharacter>(MyTarget))
	{
		AttachTargetComp = TargetCharacter->GetMesh();
	}
	else
	{
		AttachTargetComp = MyTarget->GetRootComponent();
	}
	
	if(!AttachTargetComp)
	{
		return false;
	}
	
	if (SpawnedEffectComp && SpawnedEffectComp->IsActive())
	{
		return true;
	}
	
	SpawnedEffectComp = UNiagaraFunctionLibrary::SpawnSystemAttached(
		HealEffectSystem,
		AttachTargetComp,
		AttachSocketName,
		FVector::ZeroVector,
		FRotator::ZeroRotator,
		EAttachLocation::SnapToTargetIncludingScale,
		true
	);
	
	return true;
}

bool AGCN_HealEffect::OnRemove_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters)
{
	if (SpawnedEffectComp && SpawnedEffectComp->IsActive())
	{
		SpawnedEffectComp->Deactivate();
		SpawnedEffectComp = nullptr;
	}

	return Super::OnRemove_Implementation(MyTarget, Parameters);
}




