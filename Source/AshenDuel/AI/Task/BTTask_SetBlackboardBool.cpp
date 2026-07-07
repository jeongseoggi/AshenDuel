// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_SetBlackboardBool.h"

#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UBTTask_SetBlackboardBool::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!BB)
	{
		return EBTNodeResult::Failed;
	}

	BB->SetValueAsBool(BoolKey.SelectedKeyName, bValue);
	return EBTNodeResult::Succeeded;
}
