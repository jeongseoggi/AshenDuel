// Fill out your copyright notice in the Description page of Project Settings.


#include "ADGA_Sprinting.h"

#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_WaitInputRelease.h"
#include "AshenDuel/ADGameplayTag/GameplayTags.h"
#include "AshenDuel/GAS/AttributeSet/ADAttributeSet.h"
#include "AshenDuel/GAS/AttributeSet/ADPlayerAttributeSet.h"
#include "AshenDuel/Interface/CombatInterface.h"

UADGA_Sprinting::UADGA_Sprinting()
{
}

void UADGA_Sprinting::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                    const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	if (!HasEnoughStamina())
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	
	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	
	if (ASC && SprintEffectClass && SprintConsumeEffectClass)
	{
		StaminaChangedDelegateHandle = ASC->GetGameplayAttributeValueChangeDelegate(
		UADPlayerAttributeSet::GetStaminaAttribute()).
		AddUObject(this, &UADGA_Sprinting::OnStaminaChanged);
		
		
		FGameplayEffectContextHandle ContextHandle = ASC->MakeEffectContext();
		ContextHandle.AddSourceObject(this);

		ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());
		if (CombatInterface)
		{
			ActiveEffectHandle = CombatInterface->ApplyGameplayEffectToSelf(SprintEffectClass, 1.0f, ContextHandle);
			CombatInterface->ApplyGameplayEffectToSelf(SprintConsumeEffectClass, 1.0f, ContextHandle);
		}
		
		
		UAbilityTask_WaitInputRelease* InputReleaseTask =
	UAbilityTask_WaitInputRelease::WaitInputRelease(this, true);

		if (InputReleaseTask)
		{
			InputReleaseTask->OnRelease.AddDynamic(this, &UADGA_Sprinting::OnInputReleased);
			InputReleaseTask->ReadyForActivation();
		}
	}
}

void UADGA_Sprinting::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputReleased(Handle, ActorInfo, ActivationInfo);
	
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

void UADGA_Sprinting::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo())
	{
		if (StaminaChangedDelegateHandle.IsValid())
		{
			ASC->GetGameplayAttributeValueChangeDelegate(
				UADPlayerAttributeSet::GetStaminaAttribute()
			).Remove(StaminaChangedDelegateHandle);

			StaminaChangedDelegateHandle.Reset();
		}
	}
	
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());
	if (CombatInterface)
	{
		CombatInterface->RemoveEffectWithTag(GameplayTags::State::Sprinting);
	}
	
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

bool UADGA_Sprinting::HasEnoughStamina() const
{
	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	if (!ASC) return false;
	
	const float Stamina = ASC->GetNumericAttribute(UADPlayerAttributeSet::GetStaminaAttribute());
	return Stamina > 0.0f;
}

void UADGA_Sprinting::OnStaminaChanged(const FOnAttributeChangeData& Data)
{
	if (Data.NewValue <= MinStaminaToKeepSprinting)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}
}

void UADGA_Sprinting::OnInputReleased(float TimeHeld)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}
