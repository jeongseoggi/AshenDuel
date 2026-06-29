// Fill out your copyright notice in the Description page of Project Settings.


#include "ADMainHUD.h"

#include "ADBossHealthBar.h"
#include "ADHealthBar.h"
#include "ADStaminaBar.h"

void UADMainHUD::InitAbilitySystem(UAbilitySystemComponent* InASC)
{
	ASC = InASC;

	if (!ASC)
	{
		return;
	}
	
	if (!HealthBar || !StaminaBar || !BossHealthBar) return;

	HealthBar->InitAbilitySystem(ASC);
	StaminaBar->InitAbilitySystem(ASC);
}

void UADMainHUD::InitBossAbilitySystem(UAbilitySystemComponent* InASC)
{
	BossHealthBar->InitAbilitySystem(InASC);
	BossHealthBar->SetVisibility(ESlateVisibility::Visible);
}
