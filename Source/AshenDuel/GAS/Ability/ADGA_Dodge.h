// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ADGameplayAbility.h"
#include "ADGA_Dodge.generated.h"

/**
 * 
 */
UCLASS()
class ASHENDUEL_API UADGA_Dodge : public UADGameplayAbility
{
	GENERATED_BODY()
	
public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
protected:
	UFUNCTION()
	void OnDodgeMontageEnded();
	
protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAnimMontage> DodgeMontage;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAnimMontage> SprintDodge;
};
