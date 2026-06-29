// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BossEntryTriggerActor.generated.h"

class UBoxComponent;
class AADBossCharacter;

UCLASS()
class ASHENDUEL_API ABossEntryTriggerActor : public AActor
{
	GENERATED_BODY()

public:
	ABossEntryTriggerActor();

protected:
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void OnDetectedPlayer(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
						  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
						  bool bFromSweep, const FHitResult& SweepResult);
	
protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UBoxComponent> TriggerBox;
	
	UPROPERTY(EditInstanceOnly, Category = "Boss UI")
	TObjectPtr<AADBossCharacter> TargetBoss;
};
