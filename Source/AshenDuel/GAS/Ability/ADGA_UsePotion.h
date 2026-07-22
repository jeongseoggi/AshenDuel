// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ADGameplayAbility.h"
#include "ADGA_UsePotion.generated.h"

/**
 * 
 */
UCLASS()
class ASHENDUEL_API UADGA_UsePotion : public UADGameplayAbility
{
	GENERATED_BODY()
	
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAnimMontage> DrinkMontage;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> PotionEffect;
};
