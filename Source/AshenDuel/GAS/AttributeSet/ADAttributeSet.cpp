// Fill out your copyright notice in the Description page of Project Settings.


#include "ADAttributeSet.h"

#include "GameplayEffectExtension.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UADAttributeSet::UADAttributeSet()
{
	InitMaxHealth(100.0f);
	InitHealth(GetMaxHealth());
	InitMoveSpeed(300.0f);
	InitMaxMoveSpeed(1000.0f);
	InitStamina(50.0f);
	InitMaxStamina(50.0f);
}

void UADAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
	
	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
	}
	
	if (Attribute == GetMoveSpeedAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxMoveSpeed());
	}
	
	if (Attribute == GetStaminaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxStamina());
	}
}


void UADAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	
	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth()));
	}
	
	if (Data.EvaluatedData.Attribute == GetMoveSpeedAttribute())
	{
		SetMoveSpeed(FMath::Clamp(GetMoveSpeed(), 0.0f, GetMaxMoveSpeed()));
	}
	
	if (Data.EvaluatedData.Attribute == GetStaminaAttribute())
	{
		SetStamina(FMath::Clamp(GetStamina(), 0.0f, GetMaxStamina()));
	}
	
	OwnerCharacterMoveSpeedSet(Data);
}


void UADAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);
	
	if (Attribute == GetMoveSpeedAttribute())
	{
		OwnerCharacterMoveSpeedSet(NewValue);
	}
}

void UADAttributeSet::OwnerCharacterMoveSpeedSet(const struct FGameplayEffectModCallbackData& Data)
{
	ACharacter* TargetCharacter = nullptr;
	
	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		TargetCharacter = Cast<ACharacter>(Data.Target.AbilityActorInfo->AvatarActor.Get());
	}
	
	if (TargetCharacter && TargetCharacter->GetCharacterMovement())
	{
		if (Data.EvaluatedData.Attribute == GetMoveSpeedAttribute())
		{
			TargetCharacter->GetCharacterMovement()->MaxWalkSpeed = GetMoveSpeed();
		}
	}
}

void UADAttributeSet::OwnerCharacterMoveSpeedSet(float NewValue)
{
	UAbilitySystemComponent* ASC = GetOwningAbilitySystemComponent();
	if (ASC && ASC->GetAvatarActor())
	{
		ACharacter* Character = Cast<ACharacter>(ASC->GetAvatarActor());
		if (Character && Character->GetCharacterMovement())
		{
			Character->GetCharacterMovement()->MaxWalkSpeed = NewValue;
		}
	}
}
