// Fill out your copyright notice in the Description page of Project Settings.


#include "ADGA_UsePotion.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "AshenDuel/ADGameplayTag/GameplayTags.h"
#include "AshenDuel/CoreFramework/Character/ADCharacter.h"

void UADGA_UsePotion::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                      const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                      const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	if (CheckCost(CurrentSpecHandle, CurrentActorInfo))
	{
		ApplyCost(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo);
		
		UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
			this, TEXT("Start"), DrinkMontage);
		
		ICombatInterface* CombatInt = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());
		if (CombatInt)
		{
			CombatInt->ApplyGameplayEffectToSelf(PotionDrinkSprintEffect);
		}
		
		if (MontageTask)
		{
			MontageTask->OnCompleted.AddDynamic(this, &UADGA_UsePotion::UsePotionEnded);
			MontageTask->OnInterrupted.AddDynamic(this, &UADGA_UsePotion::UsePotionEnded);
			MontageTask->ReadyForActivation();
		}
	}
}

void UADGA_UsePotion::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	ICombatInterface* CombatInt = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());
	if (CombatInt)
	{
		CombatInt->RemoveEffectWithTag(GameplayTags::State::UsingPotion);
	}
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UADGA_UsePotion::UsePotionEnded()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false,false);
}
