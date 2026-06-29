// Fill out your copyright notice in the Description page of Project Settings.


#include "ADBossHealthBar.h"

#include "AshenDuel/GAS/AttributeSet/ADBossAttributeSet.h"
#include "Components/ProgressBar.h"

void UADBossHealthBar::InitAbilitySystem(UAbilitySystemComponent* InASC)
{
 	ASC = InASC;
	
	if (!ASC)
	{
		return;
	}
	
	ASC->GetGameplayAttributeValueChangeDelegate(
		UADBossAttributeSet::GetHealthAttribute()
	).AddUObject(this, &UADBossHealthBar::OnHealthChanged);

	ASC->GetGameplayAttributeValueChangeDelegate(
		UADBossAttributeSet::GetMaxHealthAttribute()
	).AddUObject(this, &UADBossHealthBar::OnMaxHealthChanged);
	
	UpdateHealthBar();
}

void UADBossHealthBar::UpdateHealthBar()
{
	if (!ASC || !HealthBar)
	{
		return;
	}

	const float Health = ASC->GetNumericAttribute(UADBossAttributeSet::GetHealthAttribute());
	const float MaxHealth = ASC->GetNumericAttribute(UADBossAttributeSet::GetMaxHealthAttribute());

	const float Percent = MaxHealth > 0.0f ? Health / MaxHealth : 0.0f;

	if (bIsFirst)
	{
		InitializeHealthBar(Health, MaxHealth);
		return; 
	}
	
	if (!bIsIntroPlaying)
	{
		HealthBar->SetPercent(Percent);
	}
}

void UADBossHealthBar::InitializeHealthBar(float Health, float MaxHealth)
{
	bIsFirst = false;
	HealthBar->SetPercent(0.0f);
	
	IntroTargetPercent = MaxHealth > 0.0f ? Health / MaxHealth : 0.0f;
	IntroCurrentPercent = 0.0f;
	bIsIntroPlaying = true;
	
	GetWorld()->GetTimerManager().SetTimer(IntroTimerHandle, this, &UADBossHealthBar::AnimateHealthBarIntro, 0.01f, true);
}

void UADBossHealthBar::AnimateHealthBarIntro()
{
	if (!HealthBar || !GetWorld()) return;
	
	float DeltaTime = GetWorld()->GetDeltaSeconds();
	IntroCurrentPercent = FMath::FInterpConstantTo(IntroCurrentPercent, IntroTargetPercent, DeltaTime, IntroSpeed);
	
	HealthBar->SetPercent(IntroCurrentPercent);
	
	if (FMath::IsNearlyEqual(IntroCurrentPercent, IntroTargetPercent, 0.005f))
	{
		HealthBar->SetPercent(IntroTargetPercent);
		bIsIntroPlaying = false;
        
		GetWorld()->GetTimerManager().ClearTimer(IntroTimerHandle);
	}
}

void UADBossHealthBar::OnHealthChanged(const FOnAttributeChangeData& Data)
{
	UpdateHealthBar();
}

void UADBossHealthBar::OnMaxHealthChanged(const FOnAttributeChangeData& Data)
{
	UpdateHealthBar();
}
