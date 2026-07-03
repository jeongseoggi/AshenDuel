// Fill out your copyright notice in the Description page of Project Settings.


#include "ADGA_FatalAttack.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

void UADGA_FatalAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                        const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                        const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	UAbilityTask_PlayMontageAndWait* FatalMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this, NAME_None, FatalAttackMontage);
	
	if (FatalMontageTask)
	{
		FatalMontageTask->OnCompleted.AddDynamic(this, &UADGA_FatalAttack::OnFatalMontageCompleted);
		FatalMontageTask->OnInterrupted.AddDynamic(this, &UADGA_FatalAttack::OnFatalMontageCompleted);
		FatalMontageTask->ReadyForActivation();
	}
	
}

void UADGA_FatalAttack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UADGA_FatalAttack::OnFatalMontageCompleted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}
