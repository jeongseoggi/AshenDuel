// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AshenDuel/Data/AttackDataAsset.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ADDataManagerSubSystem.generated.h"

struct FGameplayTag;
class UAttackDataAsset;
/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class ASHENDUEL_API UADDataManagerSubSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override; 
	
	UFUNCTION(BlueprintCallable)
	void InitializeDataManager(UAttackDataAsset* InAsset);
	
	FComboAttackData GetAttackDataByTag(const FGameplayTag& AttackTag) const;
	
public:
	UPROPERTY(EditAnywhere, Category = "Data")
	TObjectPtr<UAttackDataAsset> PlayerAttackDataAsset;
};
