// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ADCharacterBase.h"
#include "AshenDuel/Interface/EnemyHelper.h"
#include "ADBossCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBossBossDiedDelegate);

class UADAttributeSet;
class UMotionWarpingComponent;
class USphereComponent;

UCLASS()
class ASHENDUEL_API AADBossCharacter : public AADCharacterBase, public IEnemyHelper
{
	GENERATED_BODY()

public:
	AADBossCharacter();
	FORCEINLINE UMotionWarpingComponent* GetMotionWarpingComponent() const { return MotionWarpingComponent; }
	void SetBossDeath();
protected:
	virtual void BeginPlay() override;
	
protected:
	UPROPERTY(EditDefaultsOnly)
	bool bIsDeath = false;
	
#pragma region Component
protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USphereComponent> TargetingSphereComponent;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UMotionWarpingComponent> MotionWarpingComponent;
	
#pragma endregion

#pragma region EnemyHelperInterface
	virtual void OnTargeted(bool bTargeted) override;
	virtual bool CanBeTargeted() override;
#pragma endregion
	
#pragma region ASC
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UADAttributeSet> BossAttributeSet;
#pragma endregion
	
#pragma region UI
protected:
	void ShowBossUI();
#pragma endregion
	
#pragma region Delegate
public:
	UPROPERTY(BlueprintAssignable, Category = "DelegateEvents")
	FOnBossBossDiedDelegate OnBossDied;
	
	void InitiateDeathRow();

#pragma endregion
};
