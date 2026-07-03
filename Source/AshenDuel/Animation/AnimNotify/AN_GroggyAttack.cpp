// Fill out your copyright notice in the Description page of Project Settings.


#include "AN_GroggyAttack.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AshenDuel/ADGameplayTag/GameplayTags.h"

void UAN_GroggyAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                              const FAnimNotifyEventReference& EventReference)
{
	if (!MeshComp) return;
	
	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(MeshComp->GetOwner());
	if (!ASC) return;
	
	ASC->AddLooseGameplayTag(GameplayTags::Attack::GroggyAttack);
	
	
}
