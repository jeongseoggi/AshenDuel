// Fill out your copyright notice in the Description page of Project Settings.


#include "ADPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "ADGameMode.h"
#include "AshenDuel/UI/ADMainHUD.h"
#include "Character/ADBossCharacter.h"
#include "Character/ADCharacter.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"

void AADPlayerController::RegisterBossToHUD(AADBossCharacter* BossCharacter)
{
	if (MainHUDInstance)
	{
		MainHUDInstance->InitBossAbilitySystem(BossCharacter->GetAbilitySystemComponent());
	}
}

void AADPlayerController::SetDeathInputState()
{
	SetIgnoreMoveInput(true);
}

void AADPlayerController::RespawnPlayer()
{
	UWorld* World = GetWorld();
	if (!World) return;
	
	AADGameMode* ADGM = Cast<AADGameMode>(World->GetAuthGameMode());
	if (!ADGM) return;
	
	ACharacter* MyChar = Cast<ACharacter>(GetPawn());
	
	SetIgnoreMoveInput(true);
	SetIgnoreLookInput(true);
	
	ADGM->OnPlayerDied(MyChar);
}

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
