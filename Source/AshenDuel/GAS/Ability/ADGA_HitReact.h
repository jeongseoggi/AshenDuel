// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ADGameplayAbility.h"
#include "ADGA_HitReact.generated.h"

/**
 * 
 */
UCLASS()
class ASHENDUEL_API UADGA_HitReact : public UADGameplayAbility
{
	GENERATED_BODY()
public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
protected:
	void CancelAbilities();
	
protected:
	UFUNCTION()
	void OnHitReactMontageEnded();
	
	UFUNCTION()
	void OnGuardHitMontageEnded();
	
	
protected:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UAnimMontage> HitReactMontage;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UAnimMontage> BlockHitMontage;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UAnimMontage> GuardBrokenMontage;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UGameplayEffect> DamageEffectClass;
};
