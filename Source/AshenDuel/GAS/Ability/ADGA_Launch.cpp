// Fill out your copyright notice in the Description page of Project Settings.


#include "ADGA_Launch.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "AshenDuel/CoreFramework/ADPlayerController.h"
#include "AshenDuel/CoreFramework/Character/ADCharacter.h"

void UADGA_Launch::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                   const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this,
		NAME_None, LaunchMontage);
	
	if (MontageTask)
	{
		MontageTask->OnCompleted.AddDynamic(this, &UADGA_Launch::OnLaunchCharacterMotageEnded);
		MontageTask->OnInterrupted.AddDynamic(this, &UADGA_Launch::OnLaunchCharacterMotageEnded);
		MontageTask->ReadyForActivation();
	}
	
	AADCharacter* PlayerChar = Cast<AADCharacter>(GetAvatarActorFromActorInfo());
	if (!PlayerChar) return;
	
	AADPlayerController* PC = Cast<AADPlayerController>(PlayerChar->GetController());
	if (!PC) return;
	
	PC->SetInputState(true, false);
}

void UADGA_Launch::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	AADCharacter* PlayerChar = Cast<AADCharacter>(GetAvatarActorFromActorInfo());
	if (!PlayerChar) return;
	
	AADPlayerController* PC = Cast<AADPlayerController>(PlayerChar->GetController());
	if (!PC) return;
	
	PC->SetInputState(false, false);
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UADGA_Launch::OnLaunchCharacterMotageEnded()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}
