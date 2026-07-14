// Fill out your copyright notice in the Description page of Project Settings.


#include "ADGA_Death.h"

#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "AshenDuel/AshenDuel.h"
#include "AshenDuel/ADGameplayTag/GameplayTags.h"
#include "AshenDuel/CoreFramework/ADPlayerController.h"
#include "AshenDuel/CoreFramework/Character/ADCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

void UADGA_Death::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                  const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this, NAME_None, DeathMontage, 1.0f);
	
	AADCharacter* ADChar = Cast<AADCharacter>(GetAvatarActorFromActorInfo());
	if (ADChar)
	{
		if (USkeletalMeshComponent* MeshComp = ADChar->GetMesh())
		{
			MeshComp->SetCollisionResponseToChannel(ECC_Weapon, ECR_Ignore);
		}
	}
	
	if (MontageTask)
	{
		MontageTask->OnCompleted.AddDynamic(this, &UADGA_Death::OnCompletedDeathMontage);
		
		MontageTask->OnInterrupted.AddDynamic(this, &UADGA_Death::OnInterruptedDeathMontage);
		MontageTask->OnCancelled.AddDynamic(this, &UADGA_Death::OnInterruptedDeathMontage);
       
		MontageTask->ReadyForActivation();
	}
	
	UAbilityTask_WaitGameplayEvent* WaitEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, GameplayTags::Event::Character::Ragdoll, nullptr, false, false);
	
	if (WaitEventTask)
	{
		WaitEventTask->EventReceived.AddDynamic(this, &UADGA_Death::OnRagdollEventReceived);
		WaitEventTask->ReadyForActivation();
	}
}

void UADGA_Death::OnCompletedDeathMontage()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}

void UADGA_Death::OnInterruptedDeathMontage()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UADGA_Death::DisableInput()
{
	AADCharacter* ADChar = Cast<AADCharacter>(GetAvatarActorFromActorInfo());
	if (!ADChar) return;
	
	AADPlayerController* ADPC = Cast<AADPlayerController>(ADChar->GetController());
	if (!ADPC) return;
	
	ADPC->SetInputState(true, false);
}

void UADGA_Death::OnRagdollEventReceived(FGameplayEventData Payload)
{
	EnableRagdoll();
	DisableInput();
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
	
	if (AADCharacter* ADChar = Cast<AADCharacter>(GetAvatarActorFromActorInfo()))
	{
		if (AADPlayerController* ADPC = Cast<AADPlayerController>(ADChar->GetController()))
		{
			UWorld* World = GetWorld();
			if (World)
			{
				FTimerHandle RespawnTimerHandle;
				World->GetTimerManager().SetTimer(RespawnTimerHandle, ADPC, &AADPlayerController::RespawnPlayer, 3.0f, false);
			}
		}
	}
}

void UADGA_Death::EnableRagdoll()
{
	AADCharacter* ADChar = Cast<AADCharacter>(GetAvatarActorFromActorInfo());
	if (!ADChar) return;
	
	if (UCapsuleComponent* CapsuleComp = ADChar->GetCapsuleComponent())
	{
		CapsuleComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		CapsuleComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	}
	
	if (UCharacterMovementComponent* MoveComp = ADChar->GetCharacterMovement())
	{
		MoveComp->StopMovementImmediately();
		MoveComp->DisableMovement();
	}
	
	if (USkeletalMeshComponent* MeshComp = ADChar->GetMesh())
	{
		MeshComp->SetCollisionProfileName(TEXT("Ragdoll"));
		MeshComp->SetCollisionResponseToChannel(ECC_Weapon, ECR_Ignore);
		MeshComp->SetAllBodiesSimulatePhysics(true);
		MeshComp->SetSimulatePhysics(true);
		MeshComp->WakeAllRigidBodies();
	}
}
