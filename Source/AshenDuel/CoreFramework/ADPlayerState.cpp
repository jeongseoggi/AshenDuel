// Fill out your copyright notice in the Description page of Project Settings.


#include "ADPlayerState.h"

#include "AbilitySystemComponent.h"

AADPlayerState::AADPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
}

UAbilitySystemComponent* AADPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
