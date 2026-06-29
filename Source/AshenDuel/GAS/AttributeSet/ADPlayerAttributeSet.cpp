// Fill out your copyright notice in the Description page of Project Settings.


#include "ADPlayerAttributeSet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayEffectExtension.h"
#include "AshenDuel/ADGameplayTag/GameplayTags.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerState.h"

UADPlayerAttributeSet::UADPlayerAttributeSet()
{
	InitMaxHealth(100.0f);
	InitHealth(GetMaxHealth());
	InitAttackPower(20.0f);
	InitDefense(0.5f);
	InitStamina(50.0f);
	InitMaxStamina(50.0f);
	InitMoveSpeed(300.0f);
	InitMaxMoveSpeed(1000.0f);
}

void UADPlayerAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
	
	if (Attribute == GetStaminaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxStamina());
	}
}

void UADPlayerAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	
	if (Data.EvaluatedData.Attribute == GetStaminaAttribute())
	{
		SetStamina(FMath::Clamp(GetStamina(), 0.0f, GetMaxStamina()));
	}
	
	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		if (GetHealth() <= 0.0f)
		{
			PlayerDeathProcessing(Data);
		}
	}
	
	OwnerCharacterMoveSpeedSet(Data);
}

void UADPlayerAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);
	
	if (Attribute == GetMoveSpeedAttribute())
	{
		OwnerCharacterMoveSpeedSet(NewValue);
	}
}

void UADPlayerAttributeSet::OwnerCharacterMoveSpeedSet(const struct FGameplayEffectModCallbackData& Data)
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


void UADPlayerAttributeSet::OwnerCharacterMoveSpeedSet(float NewValue)
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

void UADPlayerAttributeSet::PlayerDeathProcessing(const struct FGameplayEffectModCallbackData& Data)
{
	UAbilitySystemComponent* ASC = GetOwningAbilitySystemComponent();
	APlayerState* TargetPS = Cast<APlayerState>(GetOwningActor());
	if (!TargetPS) return;
	
	AActor* TargetActor = TargetPS->GetPawn();
	
	if (ASC && TargetActor)
	{
		FGameplayEventData EventData;
		EventData.Instigator = Data.EffectSpec.GetEffectContext().GetInstigator();
		EventData.Target = TargetActor;
		
		ASC->AddLooseGameplayTag(GameplayTags::State::Death);
		
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(TargetActor, GameplayTags::Event::Death, EventData);
	}
}
