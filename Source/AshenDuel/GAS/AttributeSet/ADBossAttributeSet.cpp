// Fill out your copyright notice in the Description page of Project Settings.


#include "ADBossAttributeSet.h"

#include "GameplayEffectExtension.h"

UADBossAttributeSet::UADBossAttributeSet()
{
	InitMaxHealth(2500.0f);
	InitHealth(GetMaxHealth());
	InitAttackPower(70.0f);
	InitDefense(5.0f);
	InitPhaseTreshold(0.5f);
}

void UADBossAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
}

void UADBossAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	
	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		if (GetHealth() <= 0.0f)
		{
			//TODO : 보스 사망 트리거
		}
	}
}

void UADBossAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);
}
