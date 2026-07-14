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
	
	AADCharacterBase* OwnerCharacter = Cast<AADCharacterBase>(GetOwner());
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
				GroggyAttackChecking(SourceASC, TargetASC);
				FGameplayEffectContextHandle EffectContext = SourceASC->MakeEffectContext();
				EffectContext.AddHitResult(Hit);
             
				FGameplayEffectSpecHandle NewHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass, 1.0f, EffectContext);
				if (NewHandle.IsValid())
				{
					float GroggyDamageToSend = CurrentAttackData.GroggyDamage;
					float AttackDmg = CurrentAttackData.AttackDmg;
                
					if (GroggyDamageToSend > 0.0f)
					{
						NewHandle.Data.Get()->SetSetByCallerMagnitude(
							GameplayTags::Data::GroggyDamage,
							GroggyDamageToSend);
					}
                
					if (AttackDmg > 0.0f)
					{
						NewHandle.Data.Get()->SetSetByCallerMagnitude(
							GameplayTags::Data::AttackDamage,
							AttackDmg);
					}
                
					SourceASC->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), TargetASC);
				}
			}
		}
	}
}

void UWeaponComponent::GroggyAttackChecking(UAbilitySystemComponent* SourceASC, UAbilitySystemComponent* TargetASC)
{
	if (!SourceASC || !TargetASC) return;

	if (SourceASC->HasMatchingGameplayTag(GameplayTags::Attack::GroggyAttack) && 
		TargetASC->HasMatchingGameplayTag(GameplayTags::State::IsGroggy))
	{
		if (!TargetASC->HasMatchingGameplayTag(GameplayTags::State::KnockDown))
		{
			FGameplayEventData Data;
		
			Data.EventTag = GameplayTags::Event::Boss::KnockDown;
			Data.Instigator = SourceASC->GetAvatarActor();
			Data.Target = TargetASC->GetAvatarActor();
		
			TargetASC->HandleGameplayEvent(GameplayTags::Event::Boss::KnockDown, &Data);
       
			UE_LOG(LogTemp, Log, TEXT("🎯 그로기 막타 적중! 보스에게 KnockDown 이벤트 발송 완료."));
		}
	}
}


