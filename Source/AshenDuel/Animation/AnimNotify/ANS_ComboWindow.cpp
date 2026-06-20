// Fill out your copyright notice in the Description page of Project Settings.


#include "ANS_ComboWindow.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "AshenDuel/ADGameplayTag/GameplayTags.h"

UANS_ComboWindow::UANS_ComboWindow()
{
}

void UANS_ComboWindow::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration,
	const FAnimNotifyEventReference& EventReference)
{
	if (!MeshComp || !MeshComp->GetOwner()) return;
	
	if (IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(MeshComp->GetOwner()))
	{
		if (UAbilitySystemComponent* ASC = ASI->GetAbilitySystemComponent())
		{
			if (IsValid(ASC))
			{
				FGameplayEventData Payload;
				Payload.EventTag = GameplayTags::Event::Combo::WindowOpen;
				Payload.Instigator = MeshComp->GetOwner();
				Payload.Target = MeshComp->GetOwner();

				ASC->HandleGameplayEvent(GameplayTags::Event::Combo::WindowOpen, &Payload);
			}
		}
	}
}

void UANS_ComboWindow::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
}
