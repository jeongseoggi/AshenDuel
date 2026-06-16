// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ADMainHUD.generated.h"

class UADStaminaBar;
class UADHealthBar;
class UAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class ASHENDUEL_API UADMainHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	void InitAbilitySystem(UAbilitySystemComponent* InASC);
	
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UADHealthBar> HealthBar;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UADStaminaBar> StaminaBar;
	
private:
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> ASC;
};
