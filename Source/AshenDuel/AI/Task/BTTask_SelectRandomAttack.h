// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlueprintBase.h"
#include "BTTask_SelectRandomAttack.generated.h"

class UADGameplayAbility;
/**
 * 
 */
UCLASS()
class ASHENDUEL_API UBTTask_SelectRandomAttack : public UBTTask_BlueprintBase
{
	GENERATED_BODY()
public:
	UBTTask_SelectRandomAttack();
	
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
protected:
	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<UADGameplayAbility>> AttackAbilities;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	FBlackboardKeySelector OptimalAttackDistanceKey;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	FBlackboardKeySelector SelectedAttackTagKey;
};
