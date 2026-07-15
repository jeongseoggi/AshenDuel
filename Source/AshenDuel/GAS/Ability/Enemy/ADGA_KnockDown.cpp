// Fill out your copyright notice in the Description page of Project Settings.


#include "ADGA_KnockDown.h"

#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayTag.h"
#include "AshenDuel/ADGameplayTag/GameplayTags.h"
#include "AshenDuel/CoreFramework/Character/ADBossCharacter.h"

void UADGA_KnockDown::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                      const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                      const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	if (!ASC) return;
	
	AADBossCharacter* BossChar = Cast<AADBossCharacter>(GetAvatarActorFromActorInfo());
	if (!BossChar) return;
	
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(BossChar);
	if (!CombatInterface) return;
	
	CombatInterface->ApplyGameplayEffectToSelf(KnockDownEffect);
	
	UAbilityTask_WaitGameplayTagRemoved* WaitTagRemovedTask = UAbilityTask_WaitGameplayTagRemoved::WaitGameplayTagRemove(
		this, GameplayTags::State::KnockDown);
	if (WaitTagRemovedTask)
	{
		WaitTagRemovedTask->Removed.AddDynamic(this, &UADGA_KnockDown::OnKnockDownDurationExpired);
		WaitTagRemovedTask->ReadyForActivation();
	}
	
	UAnimInstance* AnimInst = BossChar->GetMesh()->GetAnimInstance();
	if (AnimInst)
	{
		AnimInst->Montage_Play(KnockDownMontage);
	}

	EnableFatalZone(true);
}

void UADGA_KnockDown::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	EnableFatalZone(false);
	
	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	if (ASC)
	{
		ASC->RemoveLooseGameplayTag(GameplayTags::State::KnockDown);
	}
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UADGA_KnockDown::EnableFatalZone(bool bEnable)
{
	AADBossCharacter* BossCharacter = Cast<AADBossCharacter>(GetAvatarActorFromActorInfo());
	if (!BossCharacter) return;
	
	BossCharacter->SetFatalZoneEnabled(bEnable);
}

void UADGA_KnockDown::OnKnockDownDurationExpired()
{
	UE_LOG(LogTemp, Log, TEXT("OnKnockDownDurationExpired"));
	
	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	
	if (ASC)
	{
		if (ASC->HasMatchingGameplayTag(GameplayTags::State::Boss::IsBeingFatalAttacked))
		{
			return;
		}
	}
	
	AADBossCharacter* BossChar = Cast<AADBossCharacter>(GetAvatarActorFromActorInfo());
	if (!BossChar) return;
	
	UAnimInstance* AnimInst = BossChar->GetMesh()->GetAnimInstance();
	if (AnimInst)
	{
		AnimInst->Montage_JumpToSection(TEXT("End"));
		ASC->AddLooseGameplayTag(GameplayTags::State::Boss::IsGettingUp);
	}
	
	EndAbility(CurrentSpecHandle, CurrentActorInfo ,CurrentActivationInfo, false, false);
}
