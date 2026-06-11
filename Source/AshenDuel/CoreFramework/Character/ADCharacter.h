// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
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
class ASHENDUEL_API AADCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AADCharacter();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	FORCEINLINE UCameraComponent* GetCameraComponent() const { return Camera; }

protected:
	virtual void BeginPlay() override;
	
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
	TObjectPtr<UADInputData> InputData;
	
	UPROPERTY(EditAnywhere,Category = "Input")
	TObjectPtr<UInputMappingContext> DefaultMappingContext;
#pragma endregion
	
#pragma region Abilities
protected:
	void GiveDefaultAbilities();	
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<TSubclassOf<UGameplayAbility>> DefaultAbilities;
#pragma endregion
};
