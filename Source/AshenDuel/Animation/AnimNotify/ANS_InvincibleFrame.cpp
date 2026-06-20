// Fill out your copyright notice in the Description page of Project Settings.


#include "ANS_InvincibleFrame.h"

#include "GameplayEffect.h"
#include "AshenDuel/ADGameplayTag/GameplayTags.h"
#include "AshenDuel/CoreFramework/Character/ADCharacter.h"
#include "AshenDuel/Interface/CombatInterface.h"

void UANS_InvincibleFrame::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                       float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	if (!MeshComp || !MeshComp->GetOwner() || !InvincibleEffect) return;
	
	OwnerChar = Cast<AADCharacter>(MeshComp->GetOwner());
	if (!OwnerChar) return;
	
	if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(OwnerChar))
	{
		CombatInterface->ApplyGameplayEffectToSelf(InvincibleEffect);
	}
	
}

void UANS_InvincibleFrame::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	if (!OwnerChar) return;
	
	if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(OwnerChar))
	{
		CombatInterface->RemoveEffectWithTag(GameplayTags::State::Invincible);
	}
}
