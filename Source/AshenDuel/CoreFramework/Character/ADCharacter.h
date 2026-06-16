// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "AshenDuel/Interface/CombatInterface.h"
#include "GameFramework/Character.h"
#include "ADCharacter.generated.h"

class UWeaponComponent;
class ULockOnComponent;
class UInputMappingContext;
class UGameplayAbility;
class UADInputData;
class UInputAction;
class UCameraComponent;
class USpringArmComponent;
struct FGameplayTag;

UCLASS()
class ASHENDUEL_API AADCharacter : public ACharacter, public IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()

public:
	AADCharacter();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	FORCEINLINE UCameraComponent* GetCameraComponent() const { return Camera; }

protected:
	virtual void BeginPlay() override;
	void GiveDefaultAbilites();
	
#pragma region Default
protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USpringArmComponent> SpringArm;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UCameraComponent> Camera;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAbilitySystemComponent> ASC;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<ULockOnComponent> LockOnComponent;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UWeaponComponent> WeaponComponent;
#pragma endregion
	
#pragma region Input
public:
	void SetLockOnState(bool LockOnState);
	
protected:
	UFUNCTION()
	void Move(const FInputActionValue& Value);
	
	UFUNCTION()
	void Look(const FInputActionValue& Value);
	
	UFUNCTION()
	void LockOn(const FInputActionValue& Value);
	
	UFUNCTION()
	void Input_Attack(const FInputActionValue& Value);
	
	void Input_AbilityInputTagPressed(FGameplayTag InputTag);
	void Input_AbilityInputTagReleased(FGameplayTag InputTag);
	

	
protected:
	UPROPERTY(EditAnywhere,Category = "Input")
	TObjectPtr<UInputAction> MoveAction;
	
	UPROPERTY(EditAnywhere,Category = "Input")
	TObjectPtr<UInputAction> LookAction;
	
	UPROPERTY(EditAnywhere,Category = "Input")
	TObjectPtr<UInputAction> LockOnAction;
	
	UPROPERTY(EditAnywhere,Category = "Input")
	TObjectPtr<UInputAction> SprintAction;
	
	UPROPERTY(EditAnywhere,Category = "Input")
	TObjectPtr<UInputAction> AttackAction;
	
	UPROPERTY(EditAnywhere,Category = "Input")
	TObjectPtr<UADInputData> InputData;
	
	UPROPERTY(EditAnywhere,Category = "Input")
	TObjectPtr<UInputMappingContext> DefaultMappingContext;
#pragma endregion
	
#pragma region GAS
protected:
	void GiveDefaultAbilities();
	void ApplyStartUpEffects();
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<TSubclassOf<UGameplayAbility>> DefaultAbilities;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<TSubclassOf<UGameplayEffect>> StartupEffects;
#pragma endregion
	
	
	
#pragma region Interface
public:
	virtual void RemoveEffectWithTag(const FGameplayTag& TagToRemove) override;
	virtual FActiveGameplayEffectHandle ApplyGameplayEffectToSelf(TSubclassOf<class UGameplayEffect> EffectClass, float Level = 1, FGameplayEffectContextHandle Context = FGameplayEffectContextHandle()) override;
#pragma endregion
};
