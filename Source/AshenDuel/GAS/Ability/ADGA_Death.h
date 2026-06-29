// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ADGameplayAbility.h"
#include "ADGA_Death.generated.h"

/**
 * 
 */
UCLASS()
class ASHENDUEL_API UADGA_Death : public UADGameplayAbility
{
	GENERATED_BODY()
	
public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
	
protected:
	UFUNCTION()
	void OnCompletedDeathMontage();
	
	UFUNCTION()
	void OnInterruptedDeathMontage();
	
	void DisableInput();
	
	UFUNCTION()
	void OnRagdollEventReceived(FGameplayEventData Payload);
	
	void EnableRagdoll();

protected:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UAnimMontage> DeathMontage; 
};
