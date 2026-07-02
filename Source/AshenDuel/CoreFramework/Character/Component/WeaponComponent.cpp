#include "WeaponComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AshenDuel/AshenDuel.h"
#include "AshenDuel/ADGameplayTag/GameplayTags.h"
#include "AshenDuel/CoreFramework/Character/ADCharacter.h"
#include "AshenDuel/System/ADDataManagerSubSystem.h"
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
					FGameplayTag FoundComboTag;
					const int32 MaxComboIndex = 4;
					
					for (int32 i = 1; i <= MaxComboIndex; i++)
					{
						FString TagStr = FString::Printf(TEXT("Attack.AttackCombo%d"), i);
						FGameplayTag ComboTag = FGameplayTag::RequestGameplayTag(*TagStr);
						
						if (ComboTag.IsValid() && SourceASC->HasMatchingGameplayTag(ComboTag))
						{
							FoundComboTag = ComboTag;
							break;
						}
					}
					
					float GroggyDamageToSend = 0.0f;
					if (FoundComboTag.IsValid())
					{
						UWorld* World = GetWorld();
						if (!World) return;
						
						UGameInstance* GI = World->GetGameInstance();
						if (!GI) return;
						
						UADDataManagerSubSystem* DataManager = GI->GetSubsystem<UADDataManagerSubSystem>();
						if (!DataManager) return;
						
						FComboAttackData Data = DataManager->GetAttackDataByTag(FoundComboTag);
						GroggyDamageToSend = Data.GroggyDamage;
					}
					
					if (GroggyDamageToSend > 0.0f)
					{
						NewHandle.Data.Get()->SetSetByCallerMagnitude(
							GameplayTags::Data::GroggyDamage,
							GroggyDamageToSend);
					}
					
					SourceASC->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), TargetASC);
				}
			}
		}
	}
}



