// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ADGameplayAbility.h"
#include "ADGA_Sprinting.generated.h"

/**
 * 
 */
UCLASS()
class ASHENDUEL_API UADGA_Sprinting : public UADGameplayAbility
{
	GENERATED_BODY()
	
public:
	UADGA_Sprinting();
	
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	bool HasEnoughStamina() const;
	void OnStaminaChanged(const FOnAttributeChangeData& Data);
	
	UFUNCTION()
	void OnInputReleased(float TimeHeld);
	
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GAS|Effects")
	TSubclassOf<UGameplayEffect> SprintEffectClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GAS|Effects")
	TSubclassOf<UGameplayEffect> SprintConsumeEffectClass;
	
	UPROPERTY(EditDefaultsOnly, Category="Sprint")
	float MinStaminaToKeepSprinting = 0.1f;
	
	FActiveGameplayEffectHandle ActiveEffectHandle;
	
	FDelegateHandle StaminaChangedDelegateHandle;
	
};
