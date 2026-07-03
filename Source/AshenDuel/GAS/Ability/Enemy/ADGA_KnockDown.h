// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AshenDuel/GAS/Ability/ADGameplayAbility.h"
#include "ADGA_KnockDown.generated.h"

/**
 * 
 */
UCLASS()
class ASHENDUEL_API UADGA_KnockDown : public UADGameplayAbility
{
	GENERATED_BODY()
	
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	void EnableFatalZone(bool bEnable);
	
	UFUNCTION()
	void OnKnockDownDurationExpired();
protected:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UAnimMontage> KnockDownMontage;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UGameplayEffect> KnockDownEffect;
};

