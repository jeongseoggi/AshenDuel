// Fill out your copyright notice in the Description page of Project Settings.


#include "ADPlayerAttributeSet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayEffectExtension.h"
#include "AshenDuel/ADGameplayTag/GameplayTags.h"
#include "AshenDuel/CoreFramework/Character/ADCharacterBase.h"
#include "AshenDuel/CoreFramework/Character/Component/ReactorComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerState.h"

UADPlayerAttributeSet::UADPlayerAttributeSet()
{
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
			if (UAbilitySystemComponent* ASC = GetOwningAbilitySystemComponent())
			{
				if (ASC->HasMatchingGameplayTag(GameplayTags::State::Setup::Completed))
				{
					PlayerDeathProcessing(Data);
				}
			}
		}
		else
		{
			float HealthChangeAmount = Data.EvaluatedData.Magnitude;
			if (HealthChangeAmount < 0.0f)
			{
				AADCharacterBase* TargetActor = Cast<AADCharacterBase>(Data.Target.GetAvatarActor());
				
				FGameplayEffectContextHandle Context = Data.EffectSpec.GetContext();
				AActor* AttackerActor = Context.GetInstigator();
				
				if (TargetActor && AttackerActor)
				{
					FVector AttackerLocation = AttackerActor->GetActorLocation();
					
					if (UReactorComponent* ReactComp = TargetActor->GetReactorComponent())
					{
						ReactComp->PlayHitReaction(AttackerLocation);
					}
				}
			}
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
