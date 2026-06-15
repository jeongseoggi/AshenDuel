// Fill out your copyright notice in the Description page of Project Settings.


#include "ADGA_Attack.h"

#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "AshenDuel/ADGameplayTag/GameplayTags.h"
#include "GameFramework/Character.h"


UADGA_Attack::UADGA_Attack()
{
}

void UADGA_Attack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	
	CurrentComboIndex = 1;
	bIsComboWindowOpen = false;
	bComboBuffer = false;
	
	UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this, NAME_None, ComboMontage, 1.0f, FName("Attack1"));
	
	if (MontageTask)
	{
		MontageTask->OnCompleted.AddDynamic(this, &UADGA_Attack::OnAttackCompleted);
		MontageTask->OnInterrupted.AddDynamic(this, &UADGA_Attack::OnAttackCompleted);
		MontageTask->ReadyForActivation();
	}
	
	UAbilityTask_WaitGameplayEvent* WindowOpenTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this, GameplayTags::Event::Combo::WindowOpen);
	
	if (WindowOpenTask)
	{
		WindowOpenTask->EventReceived.AddDynamic(this, &UADGA_Attack::OnComboWindowOpened);
		WindowOpenTask->ReadyForActivation();
	}
	
	UAbilityTask_WaitGameplayEvent* InputEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
	this, GameplayTags::Event::Combo::InputPressed, nullptr, false, true);

	if (InputEventTask)
	{
		InputEventTask->EventReceived.AddDynamic(this, &UADGA_Attack::OnComboInputReceived);
		InputEventTask->ReadyForActivation();
	}
	
}

void UADGA_Attack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UADGA_Attack::ExecuteComboJump()
{
	if (CurrentComboIndex >= MaxComboCount) return; 

	CurrentComboIndex++;
	FName NextSection = FName(*FString::Printf(TEXT("Attack%d"), CurrentComboIndex));
	
	MontageJumpToSection(NextSection);
	
	bIsComboWindowOpen = false;
	bComboBuffer = false;
	
}

void UADGA_Attack::OnComboWindowOpened(FGameplayEventData Payload)
{
	bIsComboWindowOpen = true;

	if (bComboBuffer)
	{
		ExecuteComboJump();
	}
}

void UADGA_Attack::OnComboInputReceived(FGameplayEventData Payload)
{
	if (bIsComboWindowOpen)
	{
		ExecuteComboJump();
	}
	else
	{
		bComboBuffer = true;

		CreateInputWaitTask();
	}
}

void UADGA_Attack::OnAttackCompleted()
{
	UE_LOG(LogTemp, Warning, TEXT("공격 몽타주가 완전히 종료되거나 방해받아 어빌리티를 정리합니다."));
	
	CurrentComboIndex = 1;
	bIsComboWindowOpen = false;
	bComboBuffer = false;
	
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UADGA_Attack::CreateInputWaitTask()
{
	if (CurrentInputTask != nullptr) 
	{
		return;
	}

	CurrentInputTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this, GameplayTags::Event::Combo::InputPressed, nullptr, false, true);
    
	if (CurrentInputTask)
	{
		CurrentInputTask->EventReceived.AddDynamic(this, &UADGA_Attack::OnComboInputReceived);
		CurrentInputTask->ReadyForActivation();
	}
}
