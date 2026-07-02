// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "ADAttributeSet.h"
#include "AttributeSet.h"
#include "ADBossAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * 
 */
UCLASS()
class ASHENDUEL_API UADBossAttributeSet : public UADAttributeSet
{
	GENERATED_BODY()
	
public:
	UADBossAttributeSet();
	
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;
	
	
public:
	ATTRIBUTE_ACCESSORS(UADBossAttributeSet, PhaseTreshold)
	ATTRIBUTE_ACCESSORS(UADBossAttributeSet, GroggyGauge)
	ATTRIBUTE_ACCESSORS(UADBossAttributeSet, MaxGroggyGauge)
	
protected:
	UPROPERTY(BlueprintReadOnly, Category = "Primary Attributes")
	FGameplayAttributeData PhaseTreshold;
	
	UPROPERTY(BlueprintReadOnly, Category = "Primary Attributes")
	FGameplayAttributeData GroggyGauge;
	
	UPROPERTY(BlueprintReadOnly, Category = "Primary Attributes")
	FGameplayAttributeData MaxGroggyGauge;
};
