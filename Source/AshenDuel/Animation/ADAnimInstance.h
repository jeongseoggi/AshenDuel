// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ADAnimInstance.generated.h"

class UAbilitySystemComponent;
class UCharacterMovementComponent;
class AADCharacter;
/**
 * 
 */
UCLASS()
class ASHENDUEL_API UADAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	FORCEINLINE bool GetIsSprinting() const { return bIsSprinting; }
	
protected:
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	TObjectPtr<AADCharacter> OwnerCharacter;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	TObjectPtr<UCharacterMovementComponent> OwnerMovementComp;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UAbilitySystemComponent> OwnerASC;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	FVector Velocity;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	float GroundSpeed;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	float PrevGroundSpeed;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	bool bShouldMove;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	bool bHasAcceleration;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	bool bIsFalling;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	float Direction;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	bool bIsStarting;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	bool bIsSprinting;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	bool bIsDead;
};
