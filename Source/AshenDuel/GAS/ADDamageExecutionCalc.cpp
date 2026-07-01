// Fill out your copyright notice in the Description page of Project Settings.


#include "ADDamageExecutionCalc.h"
#include "AbilitySystemComponent.h"
#include "AshenDuel/ADGameplayTag/GameplayTags.h"
#include "AttributeSet/ADAttributeSet.h"
#include "AttributeSet/ADBossAttributeSet.h"
#include "AttributeSet/ADPlayerAttributeSet.h"

struct FADDamageStatInternal
{
	FGameplayEffectAttributeCaptureDefinition AttackPowerDef;
	FGameplayEffectAttributeCaptureDefinition DefenseDef;
	FGameplayEffectAttributeCaptureDefinition HealthDef;
	FGameplayEffectAttributeCaptureDefinition StaminaDef;
    
	FADDamageStatInternal()
	{
		AttackPowerDef = FGameplayEffectAttributeCaptureDefinition(
		   UADAttributeSet::GetAttackPowerAttribute(),
		   EGameplayEffectAttributeCaptureSource::Source, 
		   false
		);
		
		DefenseDef = FGameplayEffectAttributeCaptureDefinition(
		   UADAttributeSet::GetDefenseAttribute(),
		   EGameplayEffectAttributeCaptureSource::Target, 
		   false
		);
		
		HealthDef = FGameplayEffectAttributeCaptureDefinition(
		   UADAttributeSet::GetHealthAttribute(),
		   EGameplayEffectAttributeCaptureSource::Target, 
		   false
		);
		
		StaminaDef = FGameplayEffectAttributeCaptureDefinition(
		   UADPlayerAttributeSet::GetStaminaAttribute(),
		   EGameplayEffectAttributeCaptureSource::Target, 
		   false
		);
	}
};

static const FADDamageStatInternal& DamageStatDef()
{
	static FADDamageStatInternal DDef;
	return DDef;
}


UADDamageExecutionCalc::UADDamageExecutionCalc()
{
	RelevantAttributesToCapture.Add(DamageStatDef().AttackPowerDef);
	RelevantAttributesToCapture.Add(DamageStatDef().DefenseDef);
	RelevantAttributesToCapture.Add(DamageStatDef().HealthDef);
	RelevantAttributesToCapture.Add(DamageStatDef().StaminaDef);
}


void UADDamageExecutionCalc::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();
	UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	
	AActor* TargetActor = TargetASC ? TargetASC->GetAvatarActor() : nullptr;
	AActor* SourceActor = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;
	
	float SourceAttackPower = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatDef().AttackPowerDef,EvaluationParameters,SourceAttackPower);
	
	float TargetDefense = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatDef().DefenseDef, EvaluationParameters, TargetDefense);
	
	float DamageDone = SourceAttackPower - TargetDefense;
	DamageDone = FMath::Max(DamageDone, 1.0f);
	
	float CurrentHealth = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatDef().HealthDef, FAggregatorEvaluateParameters(), CurrentHealth);
	
	float CurrentStamina = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatDef().StaminaDef, FAggregatorEvaluateParameters(), CurrentStamina);
	
	bool bIsBlocking = TargetASC->HasMatchingGameplayTag(GameplayTags::State::Block);
	bool bIsParrying = TargetASC->HasMatchingGameplayTag(GameplayTags::State::IsParrying);
	float FinalDamageToApply = DamageDone;
	
	if (bIsParrying)
	{
		FinalDamageToApply = 0.0f;
		UE_LOG(LogTemp, Error, TEXT("퍼펙트 패링 성공 메시지"));
	}
	else if (bIsBlocking)
	{
		FinalDamageToApply = DamageDone * 0.5f;
		float StaminaDamage = DamageDone * 0.4f;
		
		if (StaminaDamage > 0.0f)
		{
			OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(UADPlayerAttributeSet::GetStaminaAttribute(), EGameplayModOp::Additive, -StaminaDamage));
			UE_LOG(LogTemp, Error, TEXT("일반 패링 성공 메시지 -> 체력 감소 및 스테미너 감소"));
		}
		
		if (CurrentStamina - StaminaDamage <= 0.0f)
		{
			TargetASC->AddLooseGameplayTag(GameplayTags::State::IsGuardBroken);
			UE_LOG(LogTemp, Error, TEXT("가드 브레이크 메시지"));
		}
	}	
	
	if (CurrentHealth <= FinalDamageToApply)
	{
		TargetASC->AddLooseGameplayTag(GameplayTags::State::Death);
	}
	
	if (DamageDone > 0.0f)
	{
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(UADAttributeSet::GetHealthAttribute(), EGameplayModOp::Additive, -DamageDone));
	}
}