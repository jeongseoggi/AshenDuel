// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ADPlayerController.generated.h"

class UADMainHUD;
/**
 * 
 */
UCLASS()
class ASHENDUEL_API AADPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;
	void MainHUDAddToViewport();
	void MainHUDInitialize();
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UADMainHUD> MainHUDClass;
	
	UPROPERTY()
	TObjectPtr<UADMainHUD> MainHUDInstance;
};
