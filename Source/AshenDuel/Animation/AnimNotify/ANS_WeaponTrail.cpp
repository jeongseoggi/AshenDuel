// Fill out your copyright notice in the Description page of Project Settings.


#include "ANS_WeaponTrail.h"

#include "AshenDuel/CoreFramework/Character/ADCharacter.h"
#include "AshenDuel/CoreFramework/Character/Component/WeaponComponent.h"
#include "AshenDuel/Weapon/SwordWeapon.h"

void UANS_WeaponTrail::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration,
                                   const FAnimNotifyEventReference& EventReference)
{
	if (!MeshComp || !MeshComp->GetOwner() || !TrailSystem) return;
    
	if (AADCharacter* Character = Cast<AADCharacter>(MeshComp->GetOwner()))
	{
		if (UWeaponComponent* WeaponComponent = Character->GetWeaponComponent())
		{
			if (ASwordWeapon* SwordWeapon = WeaponComponent->GetWeaponInstance())
			{
				SwordWeapon->ActivateTrail(TrailSystem, SocketName);
			}
		}
	}
}

void UANS_WeaponTrail::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	if (!MeshComp || !MeshComp->GetOwner()) return;

	if (AADCharacter* Character = Cast<AADCharacter>(MeshComp->GetOwner()))
	{
		if (UWeaponComponent* WeaponComponent = Character->GetWeaponComponent())
		{
			if (ASwordWeapon* SwordWeapon = WeaponComponent->GetWeaponInstance())
			{
				SwordWeapon->DeactivateTrail();
			}
		}
	}
}
