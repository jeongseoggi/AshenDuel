#include "WeaponComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AshenDuel/AshenDuel.h"
#include "AshenDuel/ADGameplayTag/GameplayTags.h"
#include "AshenDuel/CoreFramework/Character/ADCharacter.h"
#include "AshenDuel/Weapon/SwordWeapon.h"
#include "Kismet/KismetSystemLibrary.h"

UWeaponComponent::UWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UWeaponComponent::BeginPlay()
{
	Super::BeginPlay();
	SpawnAndAttachWeapon();
}

void UWeaponComponent::SpawnAndAttachWeapon()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = GetOwner();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	
	if (!WeaponActorClass) return;
	
	WeaponActorInst = GetWorld()->SpawnActor<ASwordWeapon>(WeaponActorClass, SpawnParams);
	
	AADCharacter* OwnerCharacter = Cast<AADCharacter>(GetOwner());
	checkf(OwnerCharacter, TEXT("Cast Failed Character"));
	
	WeaponActorInst->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, SocketName);
}


void UWeaponComponent::BeginWeaponTrace(USkeletalMeshComponent* SkelMesh, FName StartSocket, FName EndSocket)
{
	OwnerMesh = SkelMesh;
	AlreadyApplyDamageActors.Empty();
	bIsFirstTick = true;
}

void UWeaponComponent::TickWeaponTrace(FName StartSocketName, FName EndSocketName, float TraceRad, bool bDrawDebug)
{
	AActor* OwnerActor = GetOwner();
	if (!OwnerActor) return;
	
 	UWorld* World = OwnerMesh->GetWorld();
 	if (!World) return;
	
	if (!OwnerMesh) return;
	
	FVector CurrentStartLocation = OwnerMesh->GetSocketLocation(StartSocketName);
	FVector CurrentEndLocation = OwnerMesh->GetSocketLocation(EndSocketName);
	
	if (bIsFirstTick)
	{
		PreviousStartLocation = CurrentStartLocation;
		PreviousEndLocation = CurrentEndLocation;
		bIsFirstTick = false;
	}
	
	float WeaponLength = FVector::Dist(CurrentStartLocation, CurrentEndLocation);
	float CapsuleHalfHeight = (WeaponLength * 0.5f) + TraceRad;
	
	FVector CapsuleCenter = (CurrentStartLocation + CurrentEndLocation) * 0.5f;
	FVector PreviousCapsuleCenter = (PreviousStartLocation + PreviousEndLocation) * 0.5f;
	FQuat CapsuleRotation = FRotationMatrix::MakeFromZ(CurrentEndLocation - CurrentStartLocation).ToQuat();
	
	TArray<FHitResult> OutHits;
	
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(OwnerMesh->GetOwner());
	
	FCollisionShape CapsuleShape = FCollisionShape::MakeCapsule(TraceRad, CapsuleHalfHeight);
	
	bool bHit = World->SweepMultiByChannel(
		OutHits,
		PreviousCapsuleCenter, 
		CapsuleCenter,         
		CapsuleRotation,        
		ECC_Weapon,
		CapsuleShape,
		Params
	);
	
	if (bDrawDebug)
	{
		DebugAttackTrace(CapsuleCenter, CapsuleHalfHeight, TraceRad, CapsuleRotation, bHit);
	}
	
	if (bHit)
	{
		HitAttackTargetApplyGE(OutHits);
	}
	
	PreviousStartLocation = CurrentStartLocation;
	PreviousEndLocation = CurrentEndLocation;
}

void UWeaponComponent::DebugAttackTrace(FVector CapsuleCenter, float CapsuleHalfHeight, float TraceRad, FQuat CapsuleRotation, bool bHit)
{
	DrawDebugCapsule(
			OwnerMesh->GetWorld(), 
			CapsuleCenter, 
			CapsuleHalfHeight, 
			TraceRad, 
			CapsuleRotation, 
			bHit ? FColor::Red : FColor::Green, 
			false, 
			-1.0f 
		);
}

void UWeaponComponent::HitAttackTargetApplyGE(TArray<FHitResult>& OutHits)
{
	for (const FHitResult& Hit : OutHits)
	{
		AActor* HitActor = Hit.GetActor();
		if (HitActor && HitActor != GetOwner() && !AlreadyApplyDamageActors.Contains(HitActor))
		{
			AlreadyApplyDamageActors.Add(HitActor);
			
			UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(HitActor);
			UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner());
        
			if (TargetASC && SourceASC)
			{
				FGameplayEffectContextHandle EffectContext = SourceASC->MakeEffectContext();
				EffectContext.AddHitResult(Hit);

				FGameplayEffectSpecHandle NewHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass, 1.0f, EffectContext);
				if (NewHandle.IsValid())
				{
					SourceASC->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), TargetASC);
				}
			}
		}
	}
}



