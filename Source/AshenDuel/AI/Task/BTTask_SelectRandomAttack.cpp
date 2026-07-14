// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_SelectRandomAttack.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AIController.h"
#include "AshenDuel/ADGameplayTag/GameplayTags.h"
#include "AshenDuel/GAS/Ability/ADGameplayAbility.h"
#include "AshenDuel/GAS/Ability/Enemy/ADGA_BasicAttack.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_SelectRandomAttack::UBTTask_SelectRandomAttack()
{
	NodeName = TEXT("Select Random Attack");
	
	// Blackboard Key Selector가 Float 타입의 키만 필터링해서 보여주도록 제한
	OptimalAttackDistanceKey.AddFloatFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_SelectRandomAttack, OptimalAttackDistanceKey));
}

EBTNodeResult::Type UBTTask_SelectRandomAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController) return EBTNodeResult::Failed;

	APawn* ControllingPawn = AIController->GetPawn();
	if (!ControllingPawn) return EBTNodeResult::Failed;
	
	UAbilitySystemComponent* BossASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(ControllingPawn);
	if (!BossASC) return EBTNodeResult::Failed;
	
	TArray<TSubclassOf<UADGameplayAbility>> CandidatePool = AttackAbilities;
	
	if (BossASC->HasMatchingGameplayTag(GameplayTags::State::Boss::Phase2))
	{
		CandidatePool.Append(Phase2AttackAbilities); 
	}
	
	if (CandidatePool.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("[BTTask] AttackAbilities 배열이 비어있습니다."));
		return EBTNodeResult::Failed;
	}
	
	int32 RandomIndex = FMath::RandRange(0, CandidatePool.Num() - 1);
	TSubclassOf<UADGameplayAbility> SelectedAbilityClass = CandidatePool[RandomIndex];
	
	UE_LOG(LogTemp, Warning, TEXT("Random Index Log :%d"), RandomIndex);
	
	if (SelectedAbilityClass)
	{
		if (const UADGA_BasicAttack* DefaultAbility = SelectedAbilityClass->GetDefaultObject<UADGA_BasicAttack>())
		{
			float OptimalDistance = DefaultAbility->GetOptimalAttackDistance();
			FGameplayTag AttackTag = DefaultAbility->GetAbilityTriggerName();
			if (OptimalDistance <= 0.0f)
			{
				UE_LOG(LogTemp, Warning, TEXT("[BTTask] GA에 'OptimalDistance' 변수가 없습니다. 기본값 처리."));
			}

			UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
			if (BBComp)
			{
				BBComp->SetValueAsFloat(OptimalAttackDistanceKey.SelectedKeyName, OptimalDistance);
				BBComp->SetValueAsName(SelectedAttackTagKey.SelectedKeyName, AttackTag.GetTagName());
				return EBTNodeResult::Succeeded;
			}
		}
	}
	
	return EBTNodeResult::Failed;
}
