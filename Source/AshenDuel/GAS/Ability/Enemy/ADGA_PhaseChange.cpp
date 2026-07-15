// Fill out your copyright notice in the Description page of Project Settings.


#include "ADGA_PhaseChange.h"

#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "AshenDuel/ADGameplayTag/GameplayTags.h"
#include "AshenDuel/Interface/CombatInterface.h"

void UADGA_PhaseChange::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                        const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                        const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this,
		NAME_None, PhaseChangedMontage);
	
	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	
	if (MontageTask)
	{
		MontageTask->OnCompleted.AddDynamic(this, &UADGA_PhaseChange::PhaseMontageEnded);
		MontageTask->OnInterrupted.AddDynamic(this, &UADGA_PhaseChange::PhaseMontageEnded);
		MontageTask->ReadyForActivation();
	}
	
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());
	if (!CombatInterface) return;
	
	CombatInterface->ApplyGameplayEffectToSelf(PhaseChangeEffect);
	CombatInterface->ApplyGameplayEffectToSelf(GroggyRecoveryEffect);
	
	if (ASC && ASC->HasMatchingGameplayTag(GameplayTags::State::IsGroggy))
	{
		ASC->RemoveLooseGameplayTag(GameplayTags::State::IsGroggy);
	}
}

void UADGA_PhaseChange::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());
	if (!CombatInterface) return;
	
	CombatInterface->RemoveEffectWithTag(GameplayTags::State::Boss::Phase2Change);
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UADGA_PhaseChange::PhaseMontageEnded()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}
