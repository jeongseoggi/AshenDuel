// Fill out your copyright notice in the Description page of Project Settings.


#include "ADStaminaBar.h"

#include "AbilitySystemComponent.h"
#include "AshenDuel/GAS/AttributeSet/ADPlayerAttributeSet.h"
#include "Components/ProgressBar.h"

void UADStaminaBar::InitAbilitySystem(UAbilitySystemComponent* InASC)
{
	ASC = InASC;

	if (!ASC)
	{
		return;
	}

	ASC->GetGameplayAttributeValueChangeDelegate(
		UADPlayerAttributeSet::GetStaminaAttribute()
	).AddUObject(this, &UADStaminaBar::OnStaminaChanged);

	ASC->GetGameplayAttributeValueChangeDelegate(
		UADPlayerAttributeSet::GetMaxStaminaAttribute()
	).AddUObject(this, &UADStaminaBar::OnMaxStaminaChanged);

	UpdateStaminaBar();

	CurrentDisplayPercent = TargetPercent;

	if (StaminaBar)
	{
		StaminaBar->SetPercent(CurrentDisplayPercent);
	}
}

void UADStaminaBar::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
	if (!StaminaBar)
	{
		return;
	}

	CurrentDisplayPercent = FMath::FInterpTo(
		CurrentDisplayPercent,
		TargetPercent,
		InDeltaTime,
		InterpSpeed
	);

	StaminaBar->SetPercent(CurrentDisplayPercent);
}

void UADStaminaBar::UpdateStaminaBar()
{
	if (!ASC)
	{
		return;
	}

	const float Stamina = ASC->GetNumericAttribute(UADPlayerAttributeSet::GetStaminaAttribute());
	const float MaxStamina = ASC->GetNumericAttribute(UADPlayerAttributeSet::GetMaxStaminaAttribute());

	const float NewPercent = MaxStamina > 0.0f ? Stamina / MaxStamina : 0.0f;
	TargetPercent = FMath::Clamp(NewPercent, 0.0f, 1.0f);
	
	if (TargetPercent < CurrentDisplayPercent)
	{
		CurrentDisplayPercent = TargetPercent;

		if (StaminaBar)
		{
			StaminaBar->SetPercent(CurrentDisplayPercent);
		}
	}
}

void UADStaminaBar::OnStaminaChanged(const FOnAttributeChangeData& Data)
{
	UpdateStaminaBar();
}

void UADStaminaBar::OnMaxStaminaChanged(const FOnAttributeChangeData& Data)
{
	UpdateStaminaBar();
}
