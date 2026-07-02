// Fill out your copyright notice in the Description page of Project Settings.


#include "ADGA_HeavyAttack.h"

#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "AshenDuel/ADGameplayTag/GameplayTags.h"
#include "AshenDuel/CoreFramework/Character/ADCharacter.h"

void UADGA_HeavyAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                        const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                        const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	UE_LOG(LogTemp, Warning, TEXT("Heavy 활성화"));
	
	AADCharacter* PlayerCharacter = Cast<AADCharacter>(GetAvatarActorFromActorInfo());
	if (!PlayerCharacter) return;
	
	UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this, 
		TEXT("HeavyAttackTask"), 
		HeavyAttackMontage, 
		1.0f, 
		TEXT("Start")
	);
	
	if (MontageTask)
	{
		MontageTask->OnCompleted.AddDynamic(this, &UADGA_HeavyAttack::OnMontageFinished);
		MontageTask->OnBlendOut.AddDynamic(this, &UADGA_HeavyAttack::OnMontageFinished);
		MontageTask->OnInterrupted.AddDynamic(this, &UADGA_HeavyAttack::OnMontageInterrupted);
		MontageTask->OnCancelled.AddDynamic(this, &UADGA_HeavyAttack::OnMontageInterrupted);
		
		MontageTask->ReadyForActivation();
	}
	
	CharageStartEffectHandle = PlayerCharacter->ApplyGameplayEffectToSelf(CharageStartEffect);
}

void UADGA_HeavyAttack::InputReleased(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputReleased(Handle, ActorInfo, ActivationInfo);
	
	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	AADCharacter* PlayerCharacter = Cast<AADCharacter>(GetAvatarActorFromActorInfo());
	UAnimInstance* PlayerAnimInst = PlayerCharacter ? PlayerCharacter->GetMesh()->GetAnimInstance() : nullptr;
	if (!ASC || !PlayerAnimInst) return;
    
	if (ASC->HasMatchingGameplayTag(GameplayTags::State::HeavyAttackCharageEnd))
	{
		PlayerAnimInst->Montage_JumpToSection(TEXT("End"));
	}
	else
	{
		PlayerAnimInst->Montage_Stop(0.2f, HeavyAttackMontage);
	}
}

void UADGA_HeavyAttack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());
	
	if (ASC && CharageStartEffectHandle.IsValid())
	{
		ASC->RemoveActiveGameplayEffect(CharageStartEffectHandle);
		if (CombatInterface)
		{
			CombatInterface->RemoveEffectWithTag(GameplayTags::State::HeavyAttackCharageEnd);
		}
	}
	if (bWasCancelled && ASC)
	{
		FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromHandle(Handle);
		if (Spec) Spec->InputPressed = false;
	}
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UADGA_HeavyAttack::OnMontageFinished()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false,  false);
}

void UADGA_HeavyAttack::OnMontageInterrupted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false,  true);
}
