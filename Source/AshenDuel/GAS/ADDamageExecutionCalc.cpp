// Fill out your copyright notice in the Description page of Project Settings.


#include "ADDamageExecutionCalc.h"
#include "AbilitySystemComponent.h"
#include "AshenDuel/ADGameplayTag/GameplayTags.h"
#include "AshenDuel/CoreFramework/Character/ADBossCharacter.h"
#include "AshenDuel/System/ADDataManagerSubSystem.h"
#include "AttributeSet/ADAttributeSet.h"
#include "AttributeSet/ADBossAttributeSet.h"
#include "AttributeSet/ADPlayerAttributeSet.h"

struct FADDamageStatInternal
{
	FGameplayEffectAttributeCaptureDefinition AttackPowerDef;
	FGameplayEffectAttributeCaptureDefinition DefenseDef;
	FGameplayEffectAttributeCaptureDefinition HealthDef;
	FGameplayEffectAttributeCaptureDefinition StaminaDef;
	FGameplayEffectAttributeCaptureDefinition GroggyDef;
    
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
		
		GroggyDef = FGameplayEffectAttributeCaptureDefinition(
		   UADBossAttributeSet::GetGroggyGaugeAttribute(),
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
	RelevantAttributesToCapture.Add(DamageStatDef().GroggyDef);
}


#pragma optimize ("", off)
void UADDamageExecutionCalc::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
    FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
    UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();
    UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
    if (!TargetASC) return;

    const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
    FAggregatorEvaluateParameters EvaluationParameters;
    EvaluationParameters.SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
    EvaluationParameters.TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
    
    float SourceAttackPower = 0.0f;
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatDef().AttackPowerDef, EvaluationParameters, SourceAttackPower);
    SourceAttackPower += Spec.GetSetByCallerMagnitude(GameplayTags::Data::AttackDamage, false, 0.0f);
    
    float TargetDefense = 0.0f;
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatDef().DefenseDef, EvaluationParameters, TargetDefense);
    
    float DamageDone = FMath::Max(SourceAttackPower - TargetDefense, 1.0f);
	
    float CurrentHealth = 0.0f;
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatDef().HealthDef, FAggregatorEvaluateParameters(), CurrentHealth);
    
    float CurrentStamina = 0.0f;
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatDef().StaminaDef, FAggregatorEvaluateParameters(), CurrentStamina);
    
    float CurrentGroggy = 0.0f;
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatDef().GroggyDef, EvaluationParameters, CurrentGroggy);
    
    float GroggyDmg = Spec.GetSetByCallerMagnitude(GameplayTags::Data::GroggyDamage, false, 0.0f);
	
    bool bIsBlocking = TargetASC->HasMatchingGameplayTag(GameplayTags::State::Block);
    bool bIsParrying = TargetASC->HasMatchingGameplayTag(GameplayTags::State::IsParrying);
	
    float FinalDamageToApply = CalculateFinalDamage(DamageDone, TargetASC, bIsParrying, bIsBlocking);
	
    if (!bIsParrying && bIsBlocking)
    {
        HandleBlockingStamina(DamageDone, TargetASC, CurrentStamina, OutExecutionOutput);
    }
	
    if (CurrentHealth <= FinalDamageToApply)
    {
        TargetASC->AddLooseGameplayTag(GameplayTags::State::Death);
    }
    
    if (FinalDamageToApply > 0.0f)
    {
        UE_LOG(LogTemp, Log, TEXT("최종 데미지 적용: %f"), FinalDamageToApply);
        OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(UADAttributeSet::GetHealthAttribute(), EGameplayModOp::Additive, -FinalDamageToApply));
    }
	
    if (GroggyDmg > 0.0f)
    {
        HandleGroggySystem(GroggyDmg, CurrentGroggy, TargetASC, SourceASC, bIsParrying, OutExecutionOutput);
    }
}

float UADDamageExecutionCalc::CalculateFinalDamage(float BaseDamage, UAbilitySystemComponent* TargetASC,
	bool bIsParrying, bool bIsBlocking) const
{
	if (!TargetASC) return BaseDamage;
	
	FGameplayTag Phase2Tag = GameplayTags::State::Boss::Phase2Change;
	if (TargetASC->HasMatchingGameplayTag(Phase2Tag))
	{
		BaseDamage *= 0.1f; 
	}
	
	if (bIsParrying)
	{
		UE_LOG(LogTemp, Error, TEXT("퍼펙트 패링 성공 메시지"));
		return 0.0f;
	}
    
	if (bIsBlocking)
	{
		return BaseDamage * 0.5f;
	}

	return BaseDamage;
}


void UADDamageExecutionCalc::HandleBlockingStamina(float DamageDone, UAbilitySystemComponent* TargetASC,
	float CurrentStamina, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	if (!TargetASC) return;

	float StaminaDamage = DamageDone * 0.4f;
	if (StaminaDamage > 0.0f)
	{
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(UADPlayerAttributeSet::GetStaminaAttribute(), EGameplayModOp::Additive, -StaminaDamage));
		UE_LOG(LogTemp, Error, TEXT("일반 패링(가드) 성공 -> 스테미너 감소"));
	}
    
	if (CurrentStamina - StaminaDamage <= 0.0f)
	{
		TargetASC->AddLooseGameplayTag(GameplayTags::State::IsGuardBroken);
		UE_LOG(LogTemp, Error, TEXT("가드 브레이크 발생!"));
	}
}

void UADDamageExecutionCalc::HandleGroggySystem(float GroggyDmg, float CurrentGroggy,
	UAbilitySystemComponent* TargetASC, UAbilitySystemComponent* SourceASC, bool bIsParrying,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	UAbilitySystemComponent* BossASC = bIsParrying ? SourceASC : TargetASC;
	if (!BossASC || BossASC->HasMatchingGameplayTag(GameplayTags::State::IsGroggy)) return;

	AActor* BossActor = BossASC->GetAvatarActor();
	AADBossCharacter* BossCharacter = Cast<AADBossCharacter>(BossActor);
    
	float PredictedGroggy = CurrentGroggy - GroggyDmg;
    
	if (bIsParrying && SourceASC)
	{
		SourceASC->SetNumericAttributeBase(UADBossAttributeSet::GetGroggyGaugeAttribute(), PredictedGroggy);
	}
	else
	{
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(
			UADBossAttributeSet::GetGroggyGaugeAttribute(), 
			EGameplayModOp::Additive, 
			-GroggyDmg
		));
	}
    
	if (BossCharacter && (PredictedGroggy <= 0.0f))
	{
		BossCharacter->ApplyGroggy();
	}
}
#pragma optimize ("", on)
