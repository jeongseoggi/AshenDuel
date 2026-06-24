// Fill out your copyright notice in the Description page of Project Settings.


#include "ADDamageExecutionCalc.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet/ADAttributeSet.h"
#include "AttributeSet/ADBossAttributeSet.h"

struct FADDamageStatInternal
{
	FGameplayEffectAttributeCaptureDefinition AttackPowerDef;
	FGameplayEffectAttributeCaptureDefinition DefenseDef;
    
	FADDamageStatInternal()
	{
		AttackPowerDef = FGameplayEffectAttributeCaptureDefinition(
		   UADBossAttributeSet::GetAttackPowerAttribute(),
		   EGameplayEffectAttributeCaptureSource::Source, 
		   false
		);
		
		DefenseDef = FGameplayEffectAttributeCaptureDefinition(
		   UADAttributeSet::GetDefenseAttribute(),
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
	
	if (DamageDone > 0.0f)
	{
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(UADAttributeSet::GetHealthAttribute(), EGameplayModOp::Additive, -DamageDone));
	}
}
