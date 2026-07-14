// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AshenDuel/GAS/Ability/ADGameplayAbility.h"
#include "ADGA_BossAttackBase.generated.h"

/**
 * 
 */
UCLASS()
class ASHENDUEL_API UADGA_BossAttackBase : public UADGameplayAbility
{
	GENERATED_BODY()
	
public:
	FORCEINLINE float GetOptimalAttackDistance() const {return OptimalAttackDistance;}
	
protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAnimMontage> AttackMontage;
	
	UPROPERTY(EditDefaultsOnly, Category = "AI|BBValue")
	float OptimalAttackDistance;
	
	UPROPERTY(EditDefaultsOnly, Category = "AI|BBValue")
	FGameplayTag AbilityTriggerTagName;
	
	FTimerHandle WarpUpdateTimerHandle;
};
