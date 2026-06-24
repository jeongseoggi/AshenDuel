// Fill out your copyright notice in the Description page of Project Settings.


#include "ANS_MonsterAttackCollision.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemInterface.h"
#include "AshenDuel/ADGameplayTag/GameplayTags.h"
#include "Kismet/KismetSystemLibrary.h"

UANS_MonsterAttackCollision::UANS_MonsterAttackCollision()
{
	StartSocketName = TEXT("Hand_L_Start");
	EndSocketName = TEXT("Hand_L_End");
	TraceRadius = 20.0f;
	bDrawDebug = true;
}

void UANS_MonsterAttackCollision::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	if (MeshComp)
	{
		PreviousStartLocation = MeshComp->GetSocketLocation(StartSocketName);
		PreviousEndLocation = MeshComp->GetSocketLocation(EndSocketName);
		
		AlreadyApplyDamageActors.Empty();
	}
}

void UANS_MonsterAttackCollision::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
	
	if (!MeshComp) return;
	
	AActor* OwnerActor = MeshComp->GetOwner();
	if (!OwnerActor) return;
	
	FVector CurrentStartLocation = MeshComp->GetSocketLocation(StartSocketName);
	FVector CurrentEndLocation = MeshComp->GetSocketLocation(EndSocketName);
	
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(OwnerActor);
	
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
	
	TArray<FHitResult> OutHits;
	
	EDrawDebugTrace::Type DebugTraceType = bDrawDebug ? EDrawDebugTrace::ForOneFrame : EDrawDebugTrace::None;
	
	float CapsuleHalfHeight = FVector::Dist(CurrentStartLocation, CurrentEndLocation) * 0.5f;
	
	FVector CapsuleCenter = (CurrentStartLocation + CurrentEndLocation) * 0.5f;
	FVector PreviousCapsuleCenter = (PreviousStartLocation + PreviousEndLocation) * 0.5f;
	
	FQuat CapsuleRotation = FRotationMatrix::MakeFromZ(CurrentEndLocation - CurrentStartLocation).ToQuat();
	
	bool bHit = UKismetSystemLibrary::CapsuleTraceMultiForObjects(
		OwnerActor->GetWorld(),
		PreviousCapsuleCenter, 
		CapsuleCenter,         
		TraceRadius,           
		CapsuleHalfHeight,     
		ObjectTypes,
		false,
		ActorsToIgnore,
		DebugTraceType,
		OutHits,
		true
	);
	
	if (bHit)
	{
		
		for (const FHitResult& Hit : OutHits)
		{
			AActor* HitActor = Hit.GetActor();
			if (HitActor && HitActor != OwnerActor && !AlreadyApplyDamageActors.Contains(HitActor))
			{
				AlreadyApplyDamageActors.Add(HitActor);
				IAbilitySystemInterface* HitASCInterface = Cast<IAbilitySystemInterface>(HitActor);
				if (HitASCInterface)
				{
					UAbilitySystemComponent* TargetASC = HitASCInterface->GetAbilitySystemComponent();
					UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(MeshComp->GetOwner());
					
					if (TargetASC && SourceASC)
					{
						FGameplayEventData EventData;
						EventData.Instigator = OwnerActor;
						EventData.Target = HitActor;
						
						UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(HitActor, GameplayTags::GameplayEvent::HitReact, EventData);
					}
				}
			}
		}
	}
	
	PreviousStartLocation = CurrentStartLocation;
	PreviousEndLocation = CurrentEndLocation;
}

void UANS_MonsterAttackCollision::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
}
