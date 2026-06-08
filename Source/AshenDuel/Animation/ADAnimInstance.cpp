// Fill out your copyright notice in the Description page of Project Settings.


#include "ADAnimInstance.h"

#include "AshenDuel/CoreFramework/Character/ADCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UADAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	
	OwnerCharacter = Cast<AADCharacter>(TryGetPawnOwner());
	
	if (OwnerCharacter)
	{
		OwnerMovementComp = OwnerCharacter->GetCharacterMovement();
	}
}

void UADAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	if (IsValid(OwnerCharacter))
	{
		Velocity = OwnerMovementComp->Velocity;
		GroundSpeed = FVector(Velocity.X, Velocity.Y, 0).Size();
		bHasAcceleration = !OwnerMovementComp->GetCurrentAcceleration().IsNearlyZero();
		bShouldMove = bHasAcceleration;
		bIsFalling = OwnerMovementComp->IsFalling();
		Direction = CalculateDirection(Velocity, OwnerCharacter->GetActorRotation());
	}
	
}
