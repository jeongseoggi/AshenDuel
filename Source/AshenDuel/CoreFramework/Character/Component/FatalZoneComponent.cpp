// Fill out your copyright notice in the Description page of Project Settings.


#include "FatalZoneComponent.h"

#include "AbilitySystemComponent.h"
#include "AshenDuel/CoreFramework/Character/ADCharacter.h"
#include "DrawDebugHelpers.h"
#include "AshenDuel/ADGameplayTag/GameplayTags.h"

UFatalZoneComponent::UFatalZoneComponent()
{
	PrimaryComponentTick.bCanEverTick = true; 
	PrimaryComponentTick.bStartWithTickEnabled = false;
	InitBoxExtent(FVector(50.f, 50.f, 50.f));
	SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void UFatalZoneComponent::SetZoneEnabled(bool bEnable)
{
	if (bEnable) SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	else SetCollisionProfileName(TEXT("NoCollision"));
	
	SetComponentTickEnabled(bEnable);
}

void UFatalZoneComponent::TickComponent(float DeltaTime, enum ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	FVector Center = GetComponentLocation();
	FVector Extent = GetScaledBoxExtent();
	FRotator Rotation = GetComponentRotation();

	FColor BoxColor = bIsFrontZone ? FColor::Green : FColor::Red;

	DrawDebugBox(GetWorld(), Center, Extent, Rotation.Quaternion(), BoxColor, false, -1.0f, 0, 2.0f);
}

void UFatalZoneComponent::BeginPlay()
{
	Super::BeginPlay();
	OnComponentBeginOverlap.AddDynamic(this, &UFatalZoneComponent::OnZoneOverlapBegin);
	OnComponentEndOverlap.AddDynamic(this, &UFatalZoneComponent::OnZoneOverlapEnd);
}

void UFatalZoneComponent::OnZoneOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor || OtherActor == GetOwner()) return;
	
	if (AADCharacter* PlayerChar = Cast<AADCharacter>(OtherActor))
	{
		UE_LOG(LogTemp, Log, TEXT("🔥 플레이어가 %s 페이탈 존에 진입함! (앞잡 가능 상태: %s)"), 
			*GetName(), bIsFrontZone ? TEXT("앞") : TEXT("뒤"));
		
		UAbilitySystemComponent* ASC = PlayerChar->GetAbilitySystemComponent();
		if (!ASC) return;
		ASC->AddLooseGameplayTag(GameplayTags::State::CanFatal);
		PlayerChar->SetFatalTargetActor(Cast<AADCharacterBase>(GetOwner()));
		bIsFatalZoneActive = true;
	}
}

void UFatalZoneComponent::OnZoneOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!OtherActor || OtherActor == GetOwner()) return;
	
	if (AADCharacter* PlayerChar = Cast<AADCharacter>(OtherActor))
	{
		UAbilitySystemComponent* ASC = PlayerChar->GetAbilitySystemComponent();
		if (!ASC) return;
		ASC->RemoveLooseGameplayTag(GameplayTags::State::CanFatal);
		bIsFatalZoneActive = false;
	}
}


