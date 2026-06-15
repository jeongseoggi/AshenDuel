// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ADGameplayAbility.h"
#include "ADGA_Attack.generated.h"

class UAbilityTask_WaitGameplayEvent;

/**
 * 
 */
UCLASS()
class ASHENDUEL_API UADGA_Attack : public UADGameplayAbility
{
	GENERATED_BODY()
	
public:
	UADGA_Attack();
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
protected:
	
	void ExecuteComboJump();
	
	UFUNCTION() 
	void OnComboWindowOpened(FGameplayEventData Payload);
	UFUNCTION() 
	void OnComboInputReceived(FGameplayEventData Payload);
	UFUNCTION() 
	void OnAttackCompleted();
	UFUNCTION()
	void CreateInputWaitTask();
	
protected:
	UPROPERTY(EditDefaultsOnly, Category="Combat")
	TObjectPtr<UAnimMontage> ComboMontage;
	
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	int32 MaxComboCount = 4;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAbilityTask_WaitGameplayEvent> CurrentInputTask;

private:
	int32 CurrentComboIndex;
	bool bIsComboWindowOpen;
	bool bComboBuffer;
	

};

