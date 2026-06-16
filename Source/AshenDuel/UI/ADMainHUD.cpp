// Fill out your copyright notice in the Description page of Project Settings.


#include "ADMainHUD.h"

#include "ADHealthBar.h"
#include "ADStaminaBar.h"

void UADMainHUD::InitAbilitySystem(UAbilitySystemComponent* InASC)
{
	ASC = InASC;

	if (!ASC)
	{
		return;
	}

	if (HealthBar)
	{
		HealthBar->InitAbilitySystem(ASC);
	}

	if (StaminaBar)
	{
		StaminaBar->InitAbilitySystem(ASC);
	}
}
