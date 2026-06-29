// Fill out your copyright notice in the Description page of Project Settings.


#include "ADGA_BossHitReact.h"

#include "AbilitySystemComponent.h"

void UADGA_BossHitReact::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                         const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                         const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	ApplyDamage(TriggerEventData);
	
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UADGA_BossHitReact::ApplyDamage(const FGameplayEventData* TriggerEventData)
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
