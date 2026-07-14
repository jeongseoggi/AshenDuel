// Fill out your copyright notice in the Description page of Project Settings.


#include "ADBossAttributeSet.h"

#include "GameplayEffectExtension.h"
#include "AshenDuel/ADGameplayTag/GameplayTags.h"
#include "AshenDuel/CoreFramework/Character/ADBossCharacter.h"

UADBossAttributeSet::UADBossAttributeSet()
{
	InitMaxHealth(2500.0f);
	InitHealth(GetMaxHealth());
	InitAttackPower(30.0f);
	InitDefense(5.0f);
	InitPhaseTreshold(0.5f);
	InitMaxGroggyGauge(100.0f);
	InitGroggyGauge(GetMaxGroggyGauge());
}

void UADBossAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
	
	if (Attribute == GetMaxHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxGroggyGauge());
	}
	
}
#pragma optimize("", off)
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
		else
		{
			float CurrentHealth = GetHealth();
			float MaxHealthValue = GetMaxHealth();
			
			if (MaxHealthValue > 0.0f)
			{
				float HealthRatio = CurrentHealth / MaxHealthValue;
				float PhaseTresholdValue = GetPhaseTreshold();
				if (HealthRatio <= PhaseTresholdValue)
				{
					if (AADBossCharacter* BossChar = Cast<AADBossCharacter>(Data.Target.GetAvatarActor()))
					{
						if (!BossChar->GetPhase2Triggered())
						{
							BossChar->SetPhase2Triggered(true);
							Data.Target.TryActivateAbilitiesByTag(FGameplayTagContainer(GameplayTags::Event::Boss::Phase2Changed));
							
							UE_LOG(LogTemp, Log, TEXT("보스 체력 50프로 이하 감지! 2페이즈 어빌리티 발동!"));
						}
					}
				}
			}
		}
	}
	
	if (Data.EvaluatedData.Attribute == GetGroggyGaugeAttribute())
	{
		UE_LOG(LogTemp, Warning, TEXT("CurrentGroggy : %f"), GetGroggyGauge());
	}
}
#pragma optimize("", on)
void UADBossAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);
}
