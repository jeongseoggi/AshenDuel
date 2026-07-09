// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_ActivateGA.generated.h"

/**
 * 
 */
UCLASS()
class ASHENDUEL_API UBTTask_ActivateGA : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_ActivateGA();
	
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float dt) override;
	
protected:
	UPROPERTY(EditAnywhere, Category = "GAS")
	FGameplayTag GameplayAbilityTag;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	FBlackboardKeySelector SelectedAttackTagKey;
};
