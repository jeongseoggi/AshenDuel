// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "ADGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class ASHENDUEL_API UADGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UADGameplayAbility();
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag StartupInputTag;
};
