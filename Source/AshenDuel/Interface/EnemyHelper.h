// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "EnemyHelper.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UEnemyHelper : public UInterface
{
	GENERATED_BODY()
};

class ASHENDUEL_API IEnemyHelper
{
	GENERATED_BODY()
public:
	
	virtual void OnTargeted(bool bTargeted) = 0;
	virtual bool CanBeTargeted() = 0;
};
