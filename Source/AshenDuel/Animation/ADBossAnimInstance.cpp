// Fill out your copyright notice in the Description page of Project Settings.


#include "ADBossAnimInstance.h"

#include "KismetAnimationLibrary.h"
#include "AshenDuel/CoreFramework/Character/ADBossCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UADBossAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	
	OwnerCharacter = Cast<AADBossCharacter>(TryGetPawnOwner());
	
	if (OwnerCharacter)
	{
		OwnerMovementComp = OwnerCharacter->GetCharacterMovement();
	}
}

void UADBossAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	if (IsValid(OwnerCharacter) && IsValid(OwnerMovementComp))
	{
		Velocity = OwnerMovementComp->Velocity;
		GroundSpeed = FVector(Velocity.X, Velocity.Y, 0.0f).Size();
		bHasAcceleration = !OwnerMovementComp->GetCurrentAcceleration().IsNearlyZero();
		bShouldMove = bHasAcceleration || (GroundSpeed > 5.0f);
		Direction = UKismetAnimationLibrary::CalculateDirection(Velocity, OwnerCharacter->GetActorRotation());
	}
}
