// Fill out your copyright notice in the Description page of Project Settings.


#include "ADGA_HitReact.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "AshenDuel/ADGameplayTag/GameplayTags.h"

void UADGA_HitReact::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                     const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                     const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	FName TargetSection = TEXT("FrontReaction");
	
	if (TriggerEventData)
	{
		if (TriggerEventData->TargetTags.HasTagExact(GameplayTags::HitReact::Back)) TargetSection = TEXT("BackReaction");
		else if (TriggerEventData->TargetTags.HasTagExact(GameplayTags::HitReact::Right)) TargetSection = TEXT("RightReaction");
		else if (TriggerEventData->TargetTags.HasTagExact(GameplayTags::HitReact::Left)) TargetSection = TEXT("LeftReaction");
	}
	
	UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this, NAME_None, HitReactMontage, 1.0f, TargetSection);
	
	if (MontageTask)
	{
		MontageTask->OnCompleted.AddDynamic(this, &UADGA_HitReact::OnHitReactMontageEnded);
		MontageTask->OnInterrupted.AddDynamic(this, &UADGA_HitReact::OnHitReactMontageEnded);
		MontageTask->ReadyForActivation();
	}
	else
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	}
}

void UADGA_HitReact::OnHitReactMontageEnded()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
