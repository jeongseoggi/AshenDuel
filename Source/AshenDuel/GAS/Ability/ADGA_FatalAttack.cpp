// Fill out your copyright notice in the Description page of Project Settings.


#include "ADGA_FatalAttack.h"

#include "MotionWarpingComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "AshenDuel/ADGameplayTag/GameplayTags.h"
#include "AshenDuel/CoreFramework/Character/ADBossCharacter.h"
#include "AshenDuel/CoreFramework/Character/ADCharacter.h"

void UADGA_FatalAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                        const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                        const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	UAbilityTask_PlayMontageAndWait* FatalMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this, NAME_None, FatalAttackMontage);
	
	if (FatalMontageTask)
	{
		FatalMontageTask->OnCompleted.AddDynamic(this, &UADGA_FatalAttack::OnFatalMontageCompleted);
		FatalMontageTask->OnInterrupted.AddDynamic(this, &UADGA_FatalAttack::OnFatalMontageCompleted);
		FatalMontageTask->ReadyForActivation();
	}
	
	AADCharacter* ADCharacter = Cast<AADCharacter>(GetAvatarActorFromActorInfo());
	if (!ADCharacter) return;
	
	AADBossCharacter* TargetBoss = ADCharacter ? Cast<AADBossCharacter>(ADCharacter->GetFatalTargetActor()) : nullptr;
	
	if (ADCharacter && TargetBoss)
	{
		UMotionWarpingComponent* WarpingComponent = ADCharacter->GetMotionWarpingComponent();
		if (WarpingComponent)
		{
			FVector TargetLocation = TargetBoss->GetFatalZoneLocation();
			FRotator TargetRotation = TargetBoss->GetFatalZoneRotation();
			
			WarpingComponent->AddOrUpdateWarpTargetFromLocationAndRotation(TEXT("FatalAttackTarget"), TargetLocation, TargetRotation);
		}
	}
	
	ICombatInterface* TargetCombatInf = Cast<ICombatInterface>(ADCharacter->GetFatalTargetActor());
	if (!TargetCombatInf) return;
	
	TargetCombatInf->RemoveEffectWithTag(GameplayTags::State::KnockDown);
	TargetCombatInf->ApplyGameplayEffectToSelf(FatalTargetEffect);
}

void UADGA_FatalAttack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	AADCharacter* ADCharacter = Cast<AADCharacter>(GetAvatarActorFromActorInfo());
	if (!ADCharacter) return;
	
	ICombatInterface* TargetCombatInf = Cast<ICombatInterface>(ADCharacter->GetFatalTargetActor());
	if (!TargetCombatInf) return;
	
	TargetCombatInf->RemoveEffectWithTag(GameplayTags::State::KnockDown);
	ADCharacter->SetFatalTargetActor(nullptr);
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UADGA_FatalAttack::OnFatalMontageCompleted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}
