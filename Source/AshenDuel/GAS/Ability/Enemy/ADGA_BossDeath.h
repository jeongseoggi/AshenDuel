// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AshenDuel/GAS/Ability/ADGameplayAbility.h"
#include "ADGA_BossDeath.generated.h"

/**
 * 
 */
UCLASS()
class ASHENDUEL_API UADGA_BossDeath : public UADGameplayAbility
{
	GENERATED_BODY()
	
public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
	UFUNCTION()
	void OnFreezeEventReceived(FGameplayEventData Payload);
	
	UFUNCTION()
	void FinalizeDeath();
	
protected:
	void DeathProcessing();
	
	void BossCollisionSetting();
	
	
protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAnimMontage> BossDeathMontage;
};
