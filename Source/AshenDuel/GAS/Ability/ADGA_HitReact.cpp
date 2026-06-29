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
		//ApplyDamage(TriggerEventData);
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}
	
	UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this, NAME_None, HitReactMontage, 1.0f, TargetSection);
	
	CancelAbilities();
	//ApplyDamage(TriggerEventData);
	
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

void UADGA_HitReact::ApplyDamage(const FGameplayEventData* TriggerEventData)
{
	UAbilitySystemComponent* TargetASC = CurrentActorInfo->AbilitySystemComponent.Get();
	if (TargetASC && DamageEffectClass)
	{
		FGameplayEffectContextHandle EffectContext = TargetASC->MakeEffectContext();
		
		if (TriggerEventData->Instigator)
		{
			AActor* InstigatorActor = const_cast<AActor*>(TriggerEventData->Instigator.Get());
			EffectContext.AddInstigator(InstigatorActor, InstigatorActor);
		}
		
		FGameplayEffectSpecHandle SpecHandle = TargetASC->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), EffectContext);
		if (SpecHandle.IsValid())
		{
			TargetASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		}
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
