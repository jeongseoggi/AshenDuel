// Fill out your copyright notice in the Description page of Project Settings.


#include "ADPlayerState.h"

#include "AbilitySystemComponent.h"
#include "AshenDuel/GAS/Component/ADAbilitySystemComponent.h"

AADPlayerState::AADPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UADAbilitySystemComponent>(TEXT("ASC"));
}

UAbilitySystemComponent* AADPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
