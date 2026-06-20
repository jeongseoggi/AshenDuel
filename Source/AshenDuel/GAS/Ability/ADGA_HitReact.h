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
	
	
protected:
	UFUNCTION()
	void OnHitReactMontageEnded();
protected:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UAnimMontage> HitReactMontage;
};
