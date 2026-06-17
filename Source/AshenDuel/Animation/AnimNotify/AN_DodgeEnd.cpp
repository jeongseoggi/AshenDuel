// Fill out your copyright notice in the Description page of Project Settings.


#include "AN_DodgeEnd.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AshenDuel/ADGameplayTag/GameplayTags.h"

void UAN_DodgeEnd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	
	if (UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(MeshComp->GetOwner()))
	{
		ASC->HandleGameplayEvent(GameplayTags::Event::Dodge_End, nullptr);
	}
}
