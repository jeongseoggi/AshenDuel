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

void UADHealthBar::UpdateHealthBar()
{
	if (!ASC || !HealthBar)
	{
		return;
	}

	const float Health = ASC->GetNumericAttribute(UADAttributeSet::GetHealthAttribute());
	const float MaxHealth = ASC->GetNumericAttribute(UADAttributeSet::GetMaxHealthAttribute());

	const float Percent = MaxHealth > 0.0f ? Health / MaxHealth : 0.0f;

	HealthBar->SetPercent(Percent);
}

void UADHealthBar::OnHealthChanged(const FOnAttributeChangeData& Data)
{
	UpdateHealthBar();
}

void UADHealthBar::OnMaxHealthChanged(const FOnAttributeChangeData& Data)
{
	UpdateHealthBar();
}
