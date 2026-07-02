// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ADGameplayAbility.h"
#include "ADGA_HeavyAttack.generated.h"

/**
 * 
 */
UCLASS()
class ASHENDUEL_API UADGA_HeavyAttack : public UADGameplayAbility
{
	GENERATED_BODY()
	
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
	UFUNCTION()
	void OnMontageFinished();
	
	UFUNCTION()
	void OnMontageInterrupted();
	
protected:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UAnimMontage> HeavyAttackMontage;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UGameplayEffect> CharageStartEffect;
	
private:
	FActiveGameplayEffectHandle CharageStartEffectHandle;
	
};
