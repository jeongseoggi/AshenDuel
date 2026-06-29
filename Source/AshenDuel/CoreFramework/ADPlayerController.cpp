// Fill out your copyright notice in the Description page of Project Settings.


#include "ADPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
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
	
	APawn* OldCharPawn = GetPawn();
	AActor* SpawnPoint = UGameplayStatics::GetActorOfClass(World, APlayerStart::StaticClass());
	if (!SpawnPoint) return;
	
	FVector SpawnLoc = SpawnPoint->GetActorLocation();
	FRotator SpawnRot = SpawnPoint->GetActorRotation();
	
	UnPossess();
	
	if (OldCharPawn)
	{
		OldCharPawn->Destroy();
	}
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	APawn* NewPawn = World->SpawnActor<APawn>(RespawnPlayerChar, SpawnLoc, SpawnRot, SpawnParams);

	if (NewPawn)
	{
		Possess(NewPawn);
		
		SetIgnoreMoveInput(false);
		SetIgnoreLookInput(false);
	}
	
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
