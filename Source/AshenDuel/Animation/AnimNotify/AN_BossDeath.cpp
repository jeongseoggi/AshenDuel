// Fill out your copyright notice in the Description page of Project Settings.


#include "AN_BossDeath.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AshenDuel/ADGameplayTag/GameplayTags.h"

void UAN_BossDeath::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                           const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	
	if (!MeshComp) return;
	
	AActor* OwnerActor = MeshComp->GetOwner();
	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(MeshComp->GetOwner());
	if (ASC && OwnerActor)
	{
		FGameplayEventData Payload;
		Payload.Instigator = OwnerActor;
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(OwnerActor, GameplayTags::Event::Boss::DeathCompleted, Payload);
	}
}
