// Fill out your copyright notice in the Description page of Project Settings.


#include "ADMainHUD.h"

#include "AbilitySystemComponent.h"
#include "ADBossHealthBar.h"
#include "ADHealthBar.h"
#include "ADPotionWidget.h"
#include "ADStaminaBar.h"
#include "AshenDuel/CoreFramework/Character/ADBossCharacter.h"

void UADMainHUD::InitAbilitySystem(UAbilitySystemComponent* InASC)
{
	ASC = InASC;

	if (!ASC)
	{
		return;
	}
	
	if (!HealthBar || !StaminaBar || !BossHealthBar || !PotionWidget) return;

	HealthBar->InitAbilitySystem(ASC);
	StaminaBar->InitAbilitySystem(ASC);
	PotionWidget->InitAbilitySystem(ASC);
}

void UADMainHUD::InitBossAbilitySystem(UAbilitySystemComponent* InASC)
{
	BossHealthBar->InitAbilitySystem(InASC);
	BossHealthBar->SetVisibility(ESlateVisibility::Visible);
	
	if (AADBossCharacter* BossChar = Cast<AADBossCharacter>(InASC->GetAvatarActor()))
	{
		BossChar->OnBossDied.AddDynamic(this, &UADMainHUD::HandleBossHealthBar);
	}
}

void UADMainHUD::HandleBossHealthBar()
{
	BossHealthBar->SetVisibility(ESlateVisibility::Hidden);
}
