// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ADHealthBar.generated.h"

class UProgressBar;
class UAbilitySystemComponent;
struct FOnAttributeChangeData;
/**
 * 
 */
UCLASS()
class ASHENDUEL_API UADHealthBar : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void InitAbilitySystem(UAbilitySystemComponent* InASC);
	
protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
private:
	void UpdateHealthBar();
	
	void OnHealthChanged(const FOnAttributeChangeData& Data);
	void OnMaxHealthChanged(const FOnAttributeChangeData& Data);
	
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> HealthBar;
	
private:
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> ASC;
	
	float TargetPercent = 1.0f; 
	float CurrentPercent = 1.0f;
	
	UPROPERTY(EditAnywhere, Category = "UI Interp")
	float InterpSpeed = 4.0f;
};
