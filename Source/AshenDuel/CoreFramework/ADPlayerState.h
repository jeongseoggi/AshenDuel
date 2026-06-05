// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "ADPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class ASHENDUEL_API AADPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	AADPlayerState();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const;
	
#pragma region GAS
protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
#pragma endregion
};
