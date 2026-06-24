// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_ActivateGA.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "AIController.h"
#include "GameplayAbilitySpecHandle.h"
#include "AshenDuel/ADGameplayTag/GameplayTags.h"
#include "AshenDuel/CoreFramework/Character/AI/ADAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_ActivateGA::UBTTask_ActivateGA()
{
	NodeName = TEXT("Activate Gameplay Ability");
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_ActivateGA::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
	if (!AIController || !BBComp) return EBTNodeResult::Failed;
	
	APawn* OwningPawn = AIController->GetPawn();
	IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(OwningPawn);
	if (!ASI) return EBTNodeResult::Failed;
	
	UAbilitySystemComponent* ASC = ASI->GetAbilitySystemComponent();
	if (!ASC) return EBTNodeResult::Failed;
	
	BBComp->SetValueAsBool(TEXT("IsAttacking"), true);
	FGameplayAbilitySpecHandle TargetAbilityHandle;
	bool bSuccess = ASC->TryActivateAbilitiesByTag(FGameplayTagContainer(GameplayAbilityTag));
	
	if (bSuccess)
	{
		return EBTNodeResult::InProgress;
	}
	
	BBComp->SetValueAsBool(TEXT("IsAttacking"), false);
	return EBTNodeResult::Failed;
}

void UBTTask_ActivateGA::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float dt)
{
	Super::TickTask(OwnerComp, NodeMemory, dt);
	UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
	if (!BBComp) return;
	
	bool bIsAttacking = BBComp->GetValueAsBool(TEXT("IsAttacking"));
	
	if (!bIsAttacking)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
