// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ADBossAnimInstance.generated.h"

class UCharacterMovementComponent;
class AADBossCharacter;
/**
 * 
 */
UCLASS()
class ASHENDUEL_API UADBossAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
protected:
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	TObjectPtr<AADBossCharacter> OwnerCharacter;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	TObjectPtr<UCharacterMovementComponent> OwnerMovementComp;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector Velocity;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float GroundSpeed;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bHasAcceleration;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bShouldMove;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float Direction;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsChasing;
	
};
