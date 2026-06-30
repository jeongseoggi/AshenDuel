// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "AshenDuel/Interface/CombatInterface.h"
#include "GameFramework/Character.h"
#include "ADCharacterBase.generated.h"

class UWeaponComponent;
class UReactorComponent;

UCLASS()
class ASHENDUEL_API AADCharacterBase : public ACharacter, public IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()

public:
	AADCharacterBase();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	FORCEINLINE UWeaponComponent* GetWeaponComponent() const {return WeaponComponent;} 
	FORCEINLINE UReactorComponent* GetReactorComponent() const {return ReactorComponent;}
protected:
	virtual void BeginPlay() override;
	
#pragma region Component
protected:
	UPROPERTY(EditDefaultsOnly, Category = "GAS")
	TObjectPtr<UAbilitySystemComponent> ASC;
    
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TObjectPtr<UWeaponComponent> WeaponComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TObjectPtr<UReactorComponent> ReactorComponent;
#pragma endregion
	
#pragma region GAS
protected:
	void GiveDefaultAbilities();
	void ApplyStartUpEffects();
    
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GAS")
	TArray<TSubclassOf<UGameplayAbility>> DefaultAbilities;
    
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GAS")
	TArray<TSubclassOf<UGameplayEffect>> StartupEffects;
#pragma endregion
	
#pragma region CombatInterface
public:
	virtual void RemoveEffectWithTag(const FGameplayTag& TagToRemove) override;
	virtual FActiveGameplayEffectHandle ApplyGameplayEffectToSelf(TSubclassOf<class UGameplayEffect> EffectClass, float Level = 1, FGameplayEffectContextHandle Context = FGameplayEffectContextHandle()) override;
#pragma endregion
};
