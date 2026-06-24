// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_CalculateDistance.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_CalculateDistance::UBTService_CalculateDistance()
{
	NodeName = TEXT("Calculate Distance To Target");
	
	Interval = 0.1f;
	RandomDeviation = 0.0f;
}

void UBTService_CalculateDistance::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	AAIController* AIController = OwnerComp.GetAIOwner();
	UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
	
	if (!AIController || !BBComp) return;
	
	APawn* ControllerPawn = AIController->GetPawn();
	if (!ControllerPawn) return;
	
	AActor* TargetActor = Cast<AActor>(BBComp->GetValueAsObject(TargetActorKey.SelectedKeyName));
	if (!TargetActor) return;
	
	float Distance = ControllerPawn->GetDistanceTo(TargetActor);
	
	BBComp->SetValueAsFloat(DistanceKey.SelectedKeyName, Distance);
}
