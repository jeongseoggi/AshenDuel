// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "InputAction.h"
#include "ADInputData.generated.h"

class UInputAction;

USTRUCT(BlueprintType)
struct FADInputAction
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<const UInputAction> Action = nullptr;
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag InputTag;
	
	UPROPERTY(EditDefaultsOnly)
	bool bBindPressed = true;
	
	UPROPERTY(EditDefaultsOnly)
	ETriggerEvent PressedTriggerEvent = ETriggerEvent::Triggered;
	
	UPROPERTY(EditDefaultsOnly)
	bool bBindReleased = false;
	
	UPROPERTY(EditDefaultsOnly)
	ETriggerEvent ReleasedTriggerEvent = ETriggerEvent::Completed;
};


UCLASS()
class ASHENDUEL_API UADInputData : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly)
	TArray<FADInputAction> AbilityActions;
};
