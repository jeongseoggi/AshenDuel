// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_SetAIMoveSpeed.h"

#include "AIController.h"
#include "AshenDuel/CoreFramework/Character/ADBossCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

UBTTask_SetAIMoveSpeed::UBTTask_SetAIMoveSpeed()
{
	NodeName = "Set AIMove Speed";
}

EBTNodeResult::Type UBTTask_SetAIMoveSpeed::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	AADBossCharacter* ControllingPawn = AIController ? Cast<AADBossCharacter>(AIController->GetPawn()) : nullptr;
	if (!ControllingPawn) return EBTNodeResult::Failed;
	
	UCharacterMovementComponent* MovementComponent = ControllingPawn->GetCharacterMovement();
	if (!MovementComponent) return EBTNodeResult::Failed;
	
	UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
	if (!BBComp) return EBTNodeResult::Failed;
	
	MovementComponent->MaxWalkSpeed = MoveSpeedValue;
	BBComp->SetValueAsFloat(MoveSpeedKeySelector.SelectedKeyName, MoveSpeedValue);
	return EBTNodeResult::Succeeded;
}
