// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "LockOnComponent.generated.h"


class UAbilitySystemComponent;
class AADCharacter;
class UCameraComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ASHENDUEL_API ULockOnComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	ULockOnComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
							   FActorComponentTickFunction* ThisTickFunction) override;
protected:
	virtual void BeginPlay() override;
	
	
public:
	void ToggleLockOn();
	
private:
	TArray<AActor*> TargetActors;
	
	AActor* FindClosestTarget(const TArray<AActor*>& TargetActors);
	
	void LockOn();
	void StartLockOn();
	void StopLockOn();
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="LockOn|Var")
	float MinLockOnDist = 800.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="LockOn|Var")
	float LockOnReleaseDist = 1000.0f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="LockOn|Var")
	float LockOnRotateSpeed = 20.0f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="LockOn|Var")
	TEnumAsByte<EDrawDebugTrace::Type> DrawDebugType = EDrawDebugTrace::Type::ForDuration;
	
private:
	UPROPERTY()
	TObjectPtr<AActor> LockOnTargetActor;
	
private:
	UPROPERTY()
	TObjectPtr<AADCharacter> OwnerChar;
	
	UPROPERTY()
	TObjectPtr<UCameraComponent> OwnerCamera;
	
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> OwnerASC;
	
private: // Debug
	UPROPERTY()
	TArray<AActor*> ActorsToIgnore;
	TArray<FHitResult> OutHits;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	
};
