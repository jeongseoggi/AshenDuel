// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "ADCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;

UCLASS()
class ASHENDUEL_API AADCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AADCharacter();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

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
#pragma endregion
};
