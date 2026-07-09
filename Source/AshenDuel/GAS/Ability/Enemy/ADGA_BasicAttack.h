// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AshenDuel/GAS/Ability/ADGameplayAbility.h"
#include "ADGA_BasicAttack.generated.h"

/**
 * 
 */
UCLASS()
class ASHENDUEL_API UADGA_BasicAttack : public UADGameplayAbility
{
	GENERATED_BODY()
	
public:
	FORCEINLINE float GetOptimalAttackDistance() const {return OptimalAttackDistance;}
	FORCEINLINE FGameplayTag GetAbilityTriggerName() const {return AbilityTriggerTagName;}
	
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
	UFUNCTION()
	void OnAttackMontageEnded();
	void UpdateWarpTargetLoop();

protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAnimMontage> AttackMontage;
	
	UPROPERTY(EditDefaultsOnly, Category = "AI|BBValue")
	float OptimalAttackDistance;
	
	UPROPERTY(EditDefaultsOnly, Category = "AI|BBValue")
	FGameplayTag AbilityTriggerTagName;
	
	FTimerHandle WarpUpdateTimerHandle;
};
