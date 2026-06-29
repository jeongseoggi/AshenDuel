// Fill out your copyright notice in the Description page of Project Settings.


#include "AN_Death.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AshenDuel/ADGameplayTag/GameplayTags.h"

void UAN_Death::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                       const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	
	if (!MeshComp) return;
	
	AActor* OwnerActor = MeshComp->GetOwner();
	if (!OwnerActor) return;
	
	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OwnerActor);
	if (ASC)
	{
		FGameplayEventData Data;
		Data.EventTag = GameplayTags::Event::Character::Ragdoll;
		Data.Instigator = OwnerActor;
		Data.Target = OwnerActor;
		
		ASC->HandleGameplayEvent(Data.EventTag, &Data);
	}
}
