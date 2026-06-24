// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ADCharacterBase.h"
#include "AshenDuel/Interface/EnemyHelper.h"
#include "ADBossCharacter.generated.h"

class UADBossAttributeSet;
class USphereComponent;

UCLASS()
class ASHENDUEL_API AADBossCharacter : public AADCharacterBase, public IEnemyHelper
{
	GENERATED_BODY()

public:
	AADBossCharacter();

protected:
	virtual void BeginPlay() override;
	
	
#pragma region Component
protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USphereComponent> TargetingSphereComponent;
#pragma endregion

#pragma region EnemyHelperInterface
	virtual void OnTargeted(bool bTargeted);
	virtual bool CanBeTargeted();
#pragma endregion
	
#pragma region ASC
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UADBossAttributeSet> BossAttributeSet;
#pragma endregion
};
