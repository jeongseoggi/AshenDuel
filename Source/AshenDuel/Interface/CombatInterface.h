// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "UObject/Interface.h"
#include "CombatInterface.generated.h"

struct FGameplayEffectContextHandle;
// This class does not need to be modified.
UINTERFACE()
class UCombatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ASHENDUEL_API ICombatInterface
{
	GENERATED_BODY()
public:
	virtual void RemoveEffectWithTag(const FGameplayTag& TagToRemove) = 0;
	virtual FActiveGameplayEffectHandle ApplyGameplayEffectToSelf(TSubclassOf<class UGameplayEffect> EffectClass, float Level = 1, FGameplayEffectContextHandle Context = FGameplayEffectContextHandle()) = 0;
};
