// Fill out your copyright notice in the Description page of Project Settings.


#include "AN_Health.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AshenDuel/ADGameplayTag/GameplayTags.h"
#include "AshenDuel/Interface/CombatInterface.h"


void UAN_Health::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                        const FAnimNotifyEventReference& EventReference)
{
	AActor* OwnerActor = MeshComp->GetOwner();
	
	if (MeshComp && OwnerActor)
	{
		ICombatInterface* Combat = Cast<ICombatInterface>(MeshComp->GetOwner());
		UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OwnerActor);
		if (Combat)
		{
			Combat->ApplyGameplayEffectToSelf(PotionEffect);
		}
		
		if (ASC)
		{
			FGameplayTag CueTag = GameplayTags::GameplayCue::HealEffect;
			
			FGameplayCueParameters Parameters;
			Parameters.Instigator = OwnerActor;
			Parameters.EffectCauser = OwnerActor;
			
			ASC->ExecuteGameplayCue(CueTag, Parameters);
		}
	}
}
