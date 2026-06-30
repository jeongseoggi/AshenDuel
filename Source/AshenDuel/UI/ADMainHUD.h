// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ADMainHUD.generated.h"

class UADBossHealthBar;
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
	void InitBossAbilitySystem(UAbilitySystemComponent* InASC);
	
protected:
	UFUNCTION()
	void HandleBossHealthBar();
	
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UADHealthBar> HealthBar;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UADStaminaBar> StaminaBar;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UADBossHealthBar> BossHealthBar;
	
private:
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> ASC;
};
