#include "LockOnComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AshenDuel/AshenDuel.h"
#include "AshenDuel/ADGameplayTag/GameplayTags.h"
#include "AshenDuel/CoreFramework/Character/ADBossCharacter.h"
#include "AshenDuel/CoreFramework/Character/ADCharacter.h"
#include "AshenDuel/Interface/EnemyHelper.h"
#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

ULockOnComponent::ULockOnComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(COLLISION_OBJECT_TARGETING));
	ActorsToIgnore.Add(GetOwner());
}

void ULockOnComponent::PostInitProperties()
{
	Super::PostInitProperties();
	
	SetComponentTickEnabled(false);
}

void ULockOnComponent::BeginPlay()
{
	Super::BeginPlay();
	
	OwnerChar = Cast<AADCharacter>(GetOwner());
	OwnerCamera = OwnerChar->GetCameraComponent();
}

UAbilitySystemComponent* ULockOnComponent::GetOwnerASC()
{
	if (!IsValid(OwnerASC))
	{
		OwnerASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner());
	}
	return OwnerASC;
}

void ULockOnComponent::TickComponent(float DeltaTime, ELevelTick TickType,
									 FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (!LockOnTargetActor)
	{
		StopLockOn();
		return;
	}
	
	if (GetOwnerASC())
	{
		if (OwnerASC->HasMatchingGameplayTag(GameplayTags::State::Death))
		{
			StopLockOn();
			return;
		}
	}
	
	if (IEnemyHelper* EnemyHelper = Cast<IEnemyHelper>(LockOnTargetActor))
	{
		if (!EnemyHelper->CanBeTargeted())
		{
			StopLockOn();
			return;
		}
	}
	
	const float Dist = FVector::Distance(OwnerChar->GetActorLocation(), LockOnTargetActor->GetActorLocation());
	
	if (Dist > LockOnReleaseDist)
	{
		StopLockOn();
	}
	else
	{
		LockOn();
	}
}

void ULockOnComponent::ToggleLockOn()
{
	if (OwnerASC->HasMatchingGameplayTag(GameplayTags::State::LockOn))
	{
		StopLockOn();
	}
	else
	{
		StartLockOn();
	}
}

TArray<AActor*> ULockOnComponent::FindTargets()
{
	TArray<AActor*> TargetingActors;
	
	const FVector Start = OwnerChar->GetActorLocation();
	const FVector End = Start;
	
	const bool bHit = UKismetSystemLibrary::SphereTraceMultiForObjects(
		GetOwner(),
		Start,
		End,
		MinLockOnDist,
		ObjectTypes,
		false,
		ActorsToIgnore,
		DrawDebugType,
		OutHits,
		true
		);
	
	if (bHit)
	{
		for (const FHitResult& HitResult : OutHits)
		{
			AActor* HitActor = HitResult.GetActor();
			TargetingActors.Add(HitActor);
		}
	}
	
	return TargetingActors;
}

AActor* ULockOnComponent::FindClosestTarget(const TArray<AActor*>& TargetActors)
{
	float TargetDotCompareValue = 0.f;
	AActor* ClosestTargetActor = nullptr;
	FHitResult OutHit;
	
	for (AActor* TargetActor : TargetActors)
	{
		AADBossCharacter* BossChar = Cast<AADBossCharacter>(TargetActor);
		if (!BossChar) continue;
		
		const FVector Start = OwnerCamera->GetComponentLocation();
		const FVector End = BossChar->GetTargetingSphereComponent()->GetComponentLocation();
		
		const bool bHit = UKismetSystemLibrary::LineTraceSingle(
			GetOwner(),
			Start,
			End,
			UEngineTypes::ConvertToTraceType(ECC_Visibility),
			false,
			ActorsToIgnore,
			DrawDebugType,
			OutHit,
			true);
       
		if (bHit)
		{
			AActor* HitActor = OutHit.GetActor();

			const FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(OwnerChar->GetActorLocation(), HitActor->GetActorLocation());
			float CheckValue = FVector::DotProduct(OwnerCamera->GetForwardVector(), LookAtRotation.Vector());
			
			if (CheckValue > TargetDotCompareValue)
			{
				TargetDotCompareValue = CheckValue;
				ClosestTargetActor = HitActor;
			}
		}
	}
    
	return ClosestTargetActor;
}

void ULockOnComponent::LockOn()
{
	const FRotator CurrentControlRotation = OwnerChar->GetControlRotation();
	const FVector TargetLocation = LockOnTargetActor->GetActorLocation() - FVector(0.f,  0.f, 150.0f);
	const FRotator TargetLookAtRotation = UKismetMathLibrary::FindLookAtRotation(OwnerChar->GetActorLocation(), TargetLocation);
	
	const FRotator InterpRotation = FMath::RInterpTo(CurrentControlRotation, TargetLookAtRotation, GetWorld()->GetDeltaSeconds(), LockOnRotateSpeed);
	OwnerChar->GetController()->SetControlRotation(FRotator(InterpRotation.Pitch, InterpRotation.Yaw, CurrentControlRotation.Roll));
	OwnerChar->SetLockOnState(true);
}

void ULockOnComponent::StartLockOn()
{
	TArray<AActor*> TargetingActors = FindTargets();
	if (TargetingActors.IsEmpty()) return;
	
	AActor* ClosestTargetActor = FindClosestTarget(TargetingActors);
	if (!ClosestTargetActor) return;

	LockOnTargetActor = ClosestTargetActor;
	if (IEnemyHelper* Targeting = Cast<IEnemyHelper>(LockOnTargetActor))
	{
		Targeting->OnTargeted(true);
	}
	
	OwnerASC->AddLooseGameplayTag(GameplayTags::State::LockOn);
	
	OwnerChar->GetCharacterMovement()->bOrientRotationToMovement = false;
	OwnerChar->GetCharacterMovement()->bUseControllerDesiredRotation = true;
	
	SetComponentTickEnabled(true);
}

void ULockOnComponent::StopLockOn()
{
	SetComponentTickEnabled(false);
	
	if (IEnemyHelper* Targeting = Cast<IEnemyHelper>(LockOnTargetActor))
	{
		Targeting->OnTargeted(false);
	}
	LockOnTargetActor = nullptr;
	
	OwnerChar->GetCharacterMovement()->bOrientRotationToMovement = true;
	OwnerChar->GetCharacterMovement()->bUseControllerDesiredRotation = false;
	
	if (GetOwnerASC())
	{
		OwnerASC->RemoveLooseGameplayTag(GameplayTags::State::LockOn);
		OwnerChar->SetLockOnState(false);
	}
}


