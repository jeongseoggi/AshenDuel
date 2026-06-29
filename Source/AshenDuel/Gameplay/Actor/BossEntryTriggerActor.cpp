// Fill out your copyright notice in the Description page of Project Settings.
#include "BossEntryTriggerActor.h"

#include "AshenDuel/CoreFramework/ADPlayerController.h"
#include "AshenDuel/CoreFramework/Character/ADBossCharacter.h"
#include "AshenDuel/CoreFramework/Character/ADCharacter.h"
#include "AshenDuel/CoreFramework/Character/AI/ADAIController.h"
#include "Components/BoxComponent.h"


ABossEntryTriggerActor::ABossEntryTriggerActor()
{
	PrimaryActorTick.bCanEverTick = false;
	
	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	RootComponent = TriggerBox;
}

void ABossEntryTriggerActor::BeginPlay()
{
	Super::BeginPlay();
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ABossEntryTriggerActor::OnDetectedPlayer);
	
}

void ABossEntryTriggerActor::OnDetectedPlayer(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AADCharacter* PlayerChar = Cast<AADCharacter>(OtherActor);
	if (!PlayerChar) return;
	
	if (!TargetBoss) return;
	
	AADPlayerController* PlayerController = Cast<AADPlayerController>(PlayerChar->GetController());
	if (!PlayerController) return;
	
	PlayerController->RegisterBossToHUD(TargetBoss);
	TriggerBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	AADAIController* AIController = Cast<AADAIController>(TargetBoss->GetController());
	
	if (AIController)
	{
		AIController->InitializeTargetActor();
	}
}

