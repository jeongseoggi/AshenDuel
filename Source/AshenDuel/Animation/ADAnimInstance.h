// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ADAnimInstance.generated.h"

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
	
protected:
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	TObjectPtr<AADCharacter> OwnerCharacter;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	TObjectPtr<UCharacterMovementComponent> OwnerMovementComp;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	FVector Velocity;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	float GroundSpeed;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	bool bShouldMove;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	bool bHasAcceleration;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	bool bIsFalling;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	float Direction;
	
};
