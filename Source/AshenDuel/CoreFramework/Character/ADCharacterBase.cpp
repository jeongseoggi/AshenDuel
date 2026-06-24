#include "ADCharacterBase.h"

#include "AbilitySystemComponent.h"
#include "AshenDuel/CoreFramework/ADPlayerState.h"
#include "AshenDuel/GAS/Ability/ADGameplayAbility.h"
#include "Component/ReactorComponent.h"
#include "Component/WeaponComponent.h"


AADCharacterBase::AADCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;
	
	WeaponComponent = CreateDefaultSubobject<UWeaponComponent>(TEXT("WeaponComponent"));
	ReactorComponent = CreateDefaultSubobject<UReactorComponent>(TEXT("ReactorComponent"));
}

UAbilitySystemComponent* AADCharacterBase::GetAbilitySystemComponent() const
{
	return ASC;
}

void AADCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	ASC = GetAbilitySystemComponent();
}

void AADCharacterBase::GiveDefaultAbilities()
{
	check(ASC);
	
	for (TSubclassOf<UGameplayAbility> AbilityClass : DefaultAbilities)
	{
		if (ASC->FindAbilitySpecFromClass(AbilityClass))
		{
			continue;
		}
		
		FGameplayAbilitySpec Spec(AbilityClass, 1, INDEX_NONE, this);
		
		if (const UADGameplayAbility* ADAbility = Cast<UADGameplayAbility>(AbilityClass->GetDefaultObject()))
		{
			if (ADAbility->StartupInputTag.IsValid())
			{
				Spec.DynamicAbilityTags.AddTag(ADAbility->StartupInputTag);
			}
		}
		
		ASC->GiveAbility(Spec);
	}
}

void AADCharacterBase::ApplyStartUpEffects()
{
	for (auto& Effect : StartupEffects)
	{
		ApplyGameplayEffectToSelf(Effect);
	}
}

void AADCharacterBase::RemoveEffectWithTag(const FGameplayTag& TagToRemove)
{
	if (ASC)
	{
		ASC->RemoveActiveEffectsWithGrantedTags(FGameplayTagContainer(TagToRemove));
	}
}

FActiveGameplayEffectHandle AADCharacterBase::ApplyGameplayEffectToSelf(TSubclassOf<class UGameplayEffect> EffectClass,
	float Level, FGameplayEffectContextHandle Context)
{
	if (ASC && EffectClass)
	{
		FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(EffectClass, Level, Context);
		if (SpecHandle.IsValid())
		{
			return ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		}
	}
	
	return FActiveGameplayEffectHandle();
}

