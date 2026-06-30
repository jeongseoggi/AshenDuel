// Fill out your copyright notice in the Description page of Project Settings.


#include "ADGA_BossDeath.h"

#include "AIController.h"
#include "BrainComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "AshenDuel/AshenDuel.h"
#include "AshenDuel/ADGameplayTag/GameplayTags.h"
#include "AshenDuel/CoreFramework/Character/ADBossCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"

void UADGA_BossDeath::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                      const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                      const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	DeathProcessing();
	BossCollisionSetting();
	
	ACharacter* BossChar = Cast<ACharacter>(GetAvatarActorFromActorInfo());
	if (!BossChar || !BossDeathMontage)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}
	
	UAbilityTask_WaitGameplayEvent* WaitEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this, 
		GameplayTags::Event::Boss::DeathCompleted);
	
	if (WaitEventTask)
	{
		WaitEventTask->EventReceived.AddDynamic(this, &UADGA_BossDeath::OnFreezeEventReceived);
		WaitEventTask->ReadyForActivation();
	}
	
	BossChar->PlayAnimMontage(BossDeathMontage);
}

void UADGA_BossDeath::OnFreezeEventReceived(FGameplayEventData Payload)
{
	AADBossCharacter* BossChar = Cast<AADBossCharacter>(GetAvatarActorFromActorInfo());
	if (!BossChar) return;

	BossChar->SetLifeSpan(5.0f);
	BossChar->InitiateDeathRow();
	
	FTimerHandle DeathTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(DeathTimerHandle, this, &UADGA_BossDeath::FinalizeDeath, 4.9f,false);
}

void UADGA_BossDeath::FinalizeDeath()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}

void UADGA_BossDeath::DeathProcessing()
{
	AActor* AvatarActor = GetAvatarActorFromActorInfo();
	APawn* MonsterPawn = Cast<APawn>(AvatarActor);
	
	if (MonsterPawn)
	{
		AAIController* Controller = Cast<AAIController>(MonsterPawn->GetController());
		if (Controller)
		{
			Controller->StopMovement();
			if (Controller->GetBrainComponent())
			{
				Controller->GetBrainComponent()->StopLogic(TEXT("Monster is Dead"));
			}
			
			if (UBlackboardComponent* BB = Controller->GetBlackboardComponent())
			{
				BB->SetValueAsObject(TEXT("TargetActor"), nullptr);
			}
			
			Controller->UnPossess();
		}
	}
}

void UADGA_BossDeath::BossCollisionSetting()
{
	AADBossCharacter* BossChar = Cast<AADBossCharacter>(GetAvatarActorFromActorInfo());
	if (!BossChar) return;
	
	BossChar->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BossChar->GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BossChar->GetMesh()->SetCollisionResponseToChannel(ECC_Weapon, ECR_Ignore);
	BossChar->SetBossDeath();
}
