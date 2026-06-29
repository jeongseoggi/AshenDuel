// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ADAIController.generated.h"

UENUM(BlueprintType)
enum class EBossAttackDirection : uint8
{
	Left,
	Right
};

UCLASS()
class ASHENDUEL_API AADAIController : public AAIController
{
	GENERATED_BODY()

public:
	AADAIController();
	EBossAttackDirection CheckPlayerLocation();
	AActor* GetTargetActor();
	void InitializeTargetActor();

protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
	
#pragma region Default
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TObjectPtr<UBlackboardComponent> BlackboardComp;
    
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TObjectPtr<UBlackboardData> BBData;
	
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TObjectPtr<UBehaviorTree> BTAsset;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<ACharacter> PlayerChar;
#pragma endregion
};
