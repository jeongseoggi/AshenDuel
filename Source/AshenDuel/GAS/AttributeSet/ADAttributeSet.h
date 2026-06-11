// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "ADAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * 
 */
UCLASS()
class ASHENDUEL_API UADAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	UADAttributeSet();
	
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;
	void OwnerCharacterMoveSpeedSet(const struct FGameplayEffectModCallbackData& Data);

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Primary Attributes")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UADAttributeSet, Health)
	
	UPROPERTY(BlueprintReadOnly, Category = "Primary Attributes")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UADAttributeSet, MaxHealth)
	
	
	UPROPERTY(BlueprintReadOnly, Category = "Primary Attributes")
	FGameplayAttributeData MoveSpeed;
	ATTRIBUTE_ACCESSORS(UADAttributeSet, MoveSpeed)
	
	
	UPROPERTY(BlueprintReadOnly, Category = "Primary Attributes")
	FGameplayAttributeData MaxMoveSpeed;
	ATTRIBUTE_ACCESSORS(UADAttributeSet, MaxMoveSpeed)
	
	
	UPROPERTY(BlueprintReadOnly, Category = "Primary Attributes")
	FGameplayAttributeData Defense;
	ATTRIBUTE_ACCESSORS(UADAttributeSet, Defense)
	
};
