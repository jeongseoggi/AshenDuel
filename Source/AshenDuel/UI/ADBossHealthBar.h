// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ADBossHealthBar.generated.h"

class UProgressBar;
class UAbilitySystemComponent;
struct FOnAttributeChangeData;
/**
 * 
 */
UCLASS()
class ASHENDUEL_API UADBossHealthBar : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void InitAbilitySystem(UAbilitySystemComponent* InASC);
	
private:
	void UpdateHealthBar();
	void InitializeHealthBar(float Health, float MaxHealth);
	void AnimateHealthBarIntro();
	void OnHealthChanged(const FOnAttributeChangeData& Data);
	void OnMaxHealthChanged(const FOnAttributeChangeData& Data);
	
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> HealthBar;
	
private:
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> ASC;
	
	FTimerHandle IntroTimerHandle;
	
	bool bIsFirst = true;
	
	bool bIsIntroPlaying = false;
	float IntroCurrentPercent = 0.0f;
	float IntroTargetPercent = 0.0f;
	
	UPROPERTY(EditDefaultsOnly, Category = "UI Animation")
	float IntroSpeed = 1.0f;
};
