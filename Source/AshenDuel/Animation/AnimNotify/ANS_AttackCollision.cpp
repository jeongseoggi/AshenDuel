// Fill out your copyright notice in the Description page of Project Settings.


#include "ANS_AttackCollision.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemInterface.h"
#include "AshenDuel/ADGameplayTag/GameplayTags.h"
#include "AshenDuel/CoreFramework/Character/ADCharacterBase.h"
#include "AshenDuel/CoreFramework/Character/Component/WeaponComponent.h"
#include "Kismet/KismetSystemLibrary.h"

UANS_AttackCollision::UANS_AttackCollision()
{
	StartSocketName = TEXT("Hand_R_Start");
	EndSocketName = TEXT("Hand_R_End");
	TraceRadius = 20.0f;
	bDrawDebug = true;
}

void UANS_AttackCollision::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	if (MeshComp)
	{
		if (AADCharacterBase* ADCH = Cast<AADCharacterBase>(MeshComp->GetOwner()))
		{
			if (UWeaponComponent* WeaponComp = ADCH->GetWeaponComponent())
			{
				WeaponComp->BeginWeaponTrace(MeshComp, StartSocketName, EndSocketName);
			}
		}
	}
}

void UANS_AttackCollision::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
	
	if (!MeshComp || !MeshComp->GetOwner()) return;
	
	if (AADCharacterBase* ADCH = Cast<AADCharacterBase>(MeshComp->GetOwner()))
	{
		if (UWeaponComponent* WeaponComp = ADCH->GetWeaponComponent())
		{
			if (ADCH->GetWorld())
			{
				WeaponComp->TickWeaponTrace(StartSocketName, EndSocketName, TraceRadius, bDrawDebug);
			}
		}
	}
}

void UANS_AttackCollision::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
}
