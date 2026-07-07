// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlueprintBase.h"
#include "BTTask_SetBlackboardBool.generated.h"

/**
 * 
 */
UCLASS()
class ASHENDUEL_API UBTTask_SetBlackboardBool : public UBTTask_BlueprintBase
{
	GENERATED_BODY()
	
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
protected:
	UPROPERTY(EditAnywhere, Category="Blackboard")
	FBlackboardKeySelector BoolKey;

	UPROPERTY(EditAnywhere, Category="Blackboard")
	bool bValue = true;
};
