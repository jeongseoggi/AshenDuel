// Fill out your copyright notice in the Description page of Project Settings.


#include "ADGA_UsePotion.h"

#include "AshenDuel/CoreFramework/Character/ADCharacter.h"

void UADGA_UsePotion::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                      const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                      const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	if (CheckCost(CurrentSpecHandle, CurrentActorInfo))
	{
		ApplyCost(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo);
		UE_LOG(LogTemp, Warning, TEXT("Potion Using"));
		
		ICombatInterface* PlayerChar = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());
		if (PlayerChar)
		{
			PlayerChar->ApplyGameplayEffectToSelf(PotionEffect);
		}
	}
	
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false,false);
}

void UADGA_UsePotion::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
