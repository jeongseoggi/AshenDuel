// Fill out your copyright notice in the Description page of Project Settings.


#include "ADGA_HitReact.h"

#include "AbilitySystemComponent.h"
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
	
	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	if (ASC->HasMatchingGameplayTag(GameplayTags::State::Death))
	{
		CancelAbilities();
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}
	
	bool bIsGuardBroken = ASC->HasMatchingGameplayTag(GameplayTags::State::IsGuardBroken);
	if (bIsGuardBroken)
	{
		FGameplayTagContainer CancelTags;
		CancelTags.AddTag(GameplayTags::State::Block);
		ASC->CancelAbilities(&CancelTags);
		
		UAbilityTask_PlayMontageAndWait* GuardBrokenTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
			this, NAME_None, GuardBrokenMontage);
		
		if (GuardBrokenTask)
		{
			GuardBrokenTask->OnCompleted.AddDynamic(this, &UADGA_HitReact::OnGuardHitMontageEnded);
			GuardBrokenTask->OnInterrupted.AddDynamic(this, &UADGA_HitReact::OnGuardHitMontageEnded);
			GuardBrokenTask->ReadyForActivation();
			return;
		}
	}
	
	
	bool bIsBlocking = ASC->HasMatchingGameplayTag(GameplayTags::State::Block);
	
	UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this, NAME_None, bIsBlocking  ? BlockHitMontage : HitReactMontage, 1.0f, bIsBlocking ? TEXT("Default") : TargetSection);
	
	CancelAbilities();
	
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


void UADGA_HitReact::CancelAbilities()
{
	UAbilitySystemComponent* TargetASC = CurrentActorInfo->AbilitySystemComponent.Get();
	if (TargetASC)
	{
		FGameplayTagContainer CancelTags;
		CancelTags.AddTag(GameplayTags::Ability::Action::Attack);
		CancelTags.AddTag(GameplayTags::Ability::Action::Dodge);
		TargetASC->CancelAbilities(&CancelTags);
	}
}

void UADGA_HitReact::OnHitReactMontageEnded()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UADGA_HitReact::OnGuardHitMontageEnded()
{
	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	if (ASC)
	{
		ASC->RemoveLooseGameplayTag(GameplayTags::State::IsGuardBroken);
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	}
}
