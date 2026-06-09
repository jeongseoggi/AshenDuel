// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/IEnemyHelper.h"
#include "LockOnTest.generated.h"

class USphereComponent;

UCLASS()
class ASHENDUEL_API ALockOnTest : public ACharacter, public IIEnemyHelper
{
	GENERATED_BODY()

public:
	ALockOnTest();

protected:
	virtual void BeginPlay() override;
	
	virtual void OnTargeted(bool bTargeted) override;
	virtual bool CanBeTargeted() override;
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USphereComponent> TargetingSphereComponent;
};
