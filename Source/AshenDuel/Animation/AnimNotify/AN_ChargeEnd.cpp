// Fill out your copyright notice in the Description page of Project Settings.


#include "AN_ChargeEnd.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AshenDuel/ADGameplayTag/GameplayTags.h"
#include "AshenDuel/Interface/CombatInterface.h"

void UAN_ChargeEnd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                           const FAnimNotifyEventReference& EventReference)
{
	if (!MeshComp) return;
	
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(MeshComp->GetOwner());
	if (!CombatInterface) return;
	
	CombatInterface->ApplyGameplayEffectToSelf(ChargeEndedEffect);
}
