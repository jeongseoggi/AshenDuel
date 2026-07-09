// Fill out your copyright notice in the Description page of Project Settings.


#include "ADGA_BasicAttack.h"

#include "AIController.h"
#include "MotionWarpingComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "AshenDuel/CoreFramework/Character/ADBossCharacter.h"
#include "AshenDuel/CoreFramework/Character/AI/ADAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

void UADGA_BasicAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                        const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                        const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	AADAIController* AIController = Cast<AADAIController>(ActorInfo->GetAnimInstance()->GetOwningActor()->GetInstigatorController());
	if (!AIController) return;
	
	AIController->GetBlackboardComponent()->SetValueAsFloat("OptimalAttackDistance", OptimalAttackDistance);
	
	
	GetWorld()->GetTimerManager().SetTimer(WarpUpdateTimerHandle, this, &UADGA_BasicAttack::UpdateWarpTargetLoop, 0.02f, true);
	UpdateWarpTargetLoop();

	UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this, NAME_None, AttackMontage, 1.0f);
	
	if (MontageTask)
	{
		MontageTask->OnCompleted.AddDynamic(this, &UADGA_BasicAttack::OnAttackMontageEnded);
		MontageTask->OnInterrupted.AddDynamic(this, &UADGA_BasicAttack::OnAttackMontageEnded);
		MontageTask->ReadyForActivation();
	}
	else
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	}

}

void UADGA_BasicAttack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (ActorInfo && ActorInfo->OwnerActor.IsValid())
	{
		AAIController* AIController = Cast<AAIController>(ActorInfo->GetAnimInstance()->GetOwningActor()->GetInstigatorController());
		
		if (!AIController)
		{
			APawn* OwningPawn = Cast<APawn>(ActorInfo->OwnerActor.Get());
			if (OwningPawn)
			{
				AIController = Cast<AAIController>(OwningPawn->GetController());
			}
		}
		
		if (AIController)
		{
			UBlackboardComponent* BBComp = AIController->GetBlackboardComponent();
			if (BBComp)
			{
				BBComp->SetValueAsBool(TEXT("IsAttacking"), false);
				UE_LOG(LogTemp, Warning, TEXT("Boss Basic Attack Ended - IsAttacking set to FALSE"));
			}
		}
	}
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UADGA_BasicAttack::OnAttackMontageEnded()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UADGA_BasicAttack::UpdateWarpTargetLoop()
{
	AActor* AvatarActor = CurrentActorInfo->AvatarActor.Get();
	if (!AvatarActor) return;

	AADAIController* AIController = Cast<AADAIController>(AvatarActor->GetInstigatorController());
	if (!AIController) return;
	
	AADBossCharacter* BossCharacter = Cast<AADBossCharacter>(AvatarActor);
	
	UMotionWarpingComponent* MotionWarpingComp = BossCharacter->GetMotionWarpingComponent();
	AActor* TargetActor = AIController->GetTargetActor();

	if (MotionWarpingComp && TargetActor)
	{
		FVector TargetLocation = TargetActor->GetActorLocation();
		
		FRotator TargetRotation = (TargetLocation - AvatarActor->GetActorLocation()).Rotation();
		TargetRotation.Pitch = 0.0f;
		TargetRotation.Roll = 0.0f;
		
		AIController->SetControlRotation(TargetRotation);
		
		MotionWarpingComp->AddOrUpdateWarpTargetFromLocationAndRotation(TEXT("CombatTarget"), TargetLocation, TargetRotation);
	}
}
