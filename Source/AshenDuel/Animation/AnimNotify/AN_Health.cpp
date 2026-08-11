// Fill out your copyright notice in the Description page of Project Settings.


#include "AN_Health.h"

#include "AshenDuel/Interface/CombatInterface.h"


void UAN_Health::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                        const FAnimNotifyEventReference& EventReference)
{
	if (MeshComp && MeshComp->GetOwner())
	{
		ICombatInterface* Combat = Cast<ICombatInterface>(MeshComp->GetOwner());
		if (Combat)
		{
			Combat->ApplyGameplayEffectToSelf(PotionEffect);
		}
	}
}
