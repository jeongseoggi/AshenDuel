// Fill out your copyright notice in the Description page of Project Settings.


#include "ADGameMode.h"

#include "Kismet/GameplayStatics.h"

void AADGameMode::OnPlayerDied(ACharacter* DeadPlayer)
{
	if (!DeadPlayer) return;
	
	FTimerHandle ReloadTimerHandle;
	GetWorldTimerManager().SetTimer(ReloadTimerHandle, this, &AADGameMode::ExecuteLevelReload, 3.0f, false);
}

void AADGameMode::ExecuteLevelReload()
{
	FString CurrentMapName = GetWorld()->GetMapName();
	CurrentMapName.RemoveFromStart(GetWorld()->StreamingLevelsPrefix); 
	UGameplayStatics::OpenLevel(GetWorld(), FName(*CurrentMapName));
}
