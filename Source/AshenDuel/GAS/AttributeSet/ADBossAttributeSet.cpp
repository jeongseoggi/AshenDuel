// Fill out your copyright notice in the Description page of Project Settings.


#include "ADBossAttributeSet.h"

#include "GameplayEffectExtension.h"
#include "AshenDuel/ADGameplayTag/GameplayTags.h"

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
			UAbilitySystemComponent* TargetASC = GetOwningAbilitySystemComponent();
			if (TargetASC)
			{
				FGameplayEventData EventData;
				TargetASC->HandleGameplayEvent(GameplayTags::Event::Boss_Death, &EventData);
			}
		}
	}
}

void UADBossAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);
}
