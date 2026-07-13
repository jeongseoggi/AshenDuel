// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "ADDamageExecutionCalc.generated.h"

/**
 * 
 */
UCLASS()
class ASHENDUEL_API UADDamageExecutionCalc : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
	
public:
	UADDamageExecutionCalc();
	
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
	
private:
	float CalculateFinalDamage(float BaseDamage, UAbilitySystemComponent* TargetASC, bool bIsParrying, bool bIsBlocking) const;
	
	void HandleBlockingStamina(float DamageDone, UAbilitySystemComponent* TargetASC, float CurrentStamina, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const;
	
	void HandleGroggySystem(float GroggyDmg, float CurrentGroggy, UAbilitySystemComponent* TargetASC, UAbilitySystemComponent* SourceASC, bool bIsParrying, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const;
};
