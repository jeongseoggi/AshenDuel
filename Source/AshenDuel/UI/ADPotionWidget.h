// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ADPotionWidget.generated.h"

class UTextBlock;
class UImage;
struct FOnAttributeChangeData;
class UAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class ASHENDUEL_API UADPotionWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void InitAbilitySystem(UAbilitySystemComponent* InASC);
	
protected:
	void OnPotionCountValueChanged(const FOnAttributeChangeData& Data);
	
	void EmptyPotion();
	
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> PotionImage;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> PotionCountText;
	
	
private:
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> ASC;
};
