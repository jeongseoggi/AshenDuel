// Fill out your copyright notice in the Description page of Project Settings.


#include "ADHealthBar.h"

#include "Components/ProgressBar.h"
#include "AbilitySystemComponent.h"
#include "AshenDuel/GAS/AttributeSet/ADAttributeSet.h"

void UADHealthBar::InitAbilitySystem(UAbilitySystemComponent* InASC)
{
	ASC = InASC;

	if (!ASC)
	{
		return;
	}

	ASC->GetGameplayAttributeValueChangeDelegate(
		UADAttributeSet::GetHealthAttribute()
	).AddUObject(this, &UADHealthBar::OnHealthChanged);

	ASC->GetGameplayAttributeValueChangeDelegate(
		UADAttributeSet::GetMaxHealthAttribute()
	).AddUObject(this, &UADHealthBar::OnMaxHealthChanged);

	UpdateHealthBar();
}

void UADHealthBar::NativeConstruct()
{
	Super::NativeConstruct();
}

void UADHealthBar::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
	if (!FMath::IsNearlyEqual(CurrentPercent, TargetPercent, 0.0001f))
	{
		CurrentPercent = FMath::FInterpTo(CurrentPercent, TargetPercent, InDeltaTime, InterpSpeed);

		if (HealthBar)
		{
			HealthBar->SetPercent(CurrentPercent);
		}
	}
	else
	{
		if (CurrentPercent != TargetPercent)
		{
			CurrentPercent = TargetPercent;
			if (HealthBar)
			{
				HealthBar->SetPercent(CurrentPercent);
			}
		}
	}
}

void UADHealthBar::UpdateHealthBar()
{
	if (!ASC || !HealthBar)
	{
		return;
	}

	const float Health = ASC->GetNumericAttribute(UADAttributeSet::GetHealthAttribute());
	const float MaxHealth = ASC->GetNumericAttribute(UADAttributeSet::GetMaxHealthAttribute());
	TargetPercent = MaxHealth > 0.0f ? FMath::Clamp(Health / MaxHealth, 0.0f, 1.0f) : 0.0f;
}

void UADHealthBar::OnHealthChanged(const FOnAttributeChangeData& Data)
{
	UpdateHealthBar();
}

void UADHealthBar::OnMaxHealthChanged(const FOnAttributeChangeData& Data)
{
	UpdateHealthBar();
}
