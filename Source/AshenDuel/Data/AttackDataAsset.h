// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "AttackDataAsset.generated.h"


USTRUCT(BlueprintType)
struct FComboAttackData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float GroggyDamage = 0.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float StaminaCost = 0.0f;
	
};


UCLASS()
class ASHENDUEL_API UAttackDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	FComboAttackData GetComboData(const FGameplayTag& AttackTag) const
	{
		const FComboAttackData* FoundData = ComboDataMap.Find(AttackTag);
		if (FoundData)
		{
			return *FoundData;
		}
		
		return FComboAttackData();
	}
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat", meta = (ForceMapLayout))
	TMap<FGameplayTag,FComboAttackData> ComboDataMap;
	
	
};
