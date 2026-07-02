// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "ADProjectSettings.generated.h"

/**
 * 
 */
UCLASS(Config=Game, DefaultConfig, meta=(DisplayName="Ashenduel Combat Settings"))
class ASHENDUEL_API UADProjectSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UADProjectSettings();
	
	UPROPERTY(Config, EditAnywhere, Category = "Data", meta = (AllowedClasses = "/Script/Ashenduel.AttackDataAsset"))
	FSoftObjectPath GlobalAttackDataAssetPath;
};
