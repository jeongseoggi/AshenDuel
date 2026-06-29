// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ADAttributeSet.h"
#include "ADPlayerAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * 
 */
UCLASS()
class ASHENDUEL_API UADPlayerAttributeSet : public UADAttributeSet
{
	GENERATED_BODY()
	
public:
	UADPlayerAttributeSet();
	
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;
	void OwnerCharacterMoveSpeedSet(const struct FGameplayEffectModCallbackData& Data);
	void OwnerCharacterMoveSpeedSet(float NewValue);
	
protected:
	void PlayerDeathProcessing(const struct FGameplayEffectModCallbackData& Data);
	
public:
	ATTRIBUTE_ACCESSORS(UADPlayerAttributeSet, Stamina)
	ATTRIBUTE_ACCESSORS(UADPlayerAttributeSet, MaxStamina)
	
protected:
	UPROPERTY(BlueprintReadOnly, Category = "Primary Attributes")
	FGameplayAttributeData Stamina;
	
	UPROPERTY(BlueprintReadOnly, Category = "Primary Attributes")
	FGameplayAttributeData MaxStamina;
};
