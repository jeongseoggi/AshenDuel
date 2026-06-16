// Fill out your copyright notice in the Description page of Project Settings.


#include "ADPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AshenDuel/UI/ADMainHUD.h"

void AADPlayerController::BeginPlay()
{
	Super::BeginPlay();
	if (!IsLocalController()) return;
	MainHUDAddToViewport();
}

void AADPlayerController::MainHUDAddToViewport()
{
	if (!MainHUDClass) return;
	
	MainHUDInstance = CreateWidget<UADMainHUD>(this, MainHUDClass);
	if (!MainHUDInstance) return;
	
	MainHUDInstance->AddToViewport();
	MainHUDInitialize();
}

void AADPlayerController::MainHUDInitialize()
{
	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn());
	if (!ASC || !MainHUDInstance) return;
	
	MainHUDInstance->InitAbilitySystem(ASC);
}
