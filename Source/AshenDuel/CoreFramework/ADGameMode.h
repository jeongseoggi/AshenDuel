// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ADGameMode.generated.h"

/**
 * 
 */
UCLASS()
class ASHENDUEL_API AADGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	void OnPlayerDied(ACharacter* DeadPlayer);
	
private:
	void ExecuteLevelReload();
};
