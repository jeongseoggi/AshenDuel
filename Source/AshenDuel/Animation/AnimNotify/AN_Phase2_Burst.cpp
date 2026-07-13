// Fill out your copyright notice in the Description page of Project Settings.


#include "AN_Phase2_Burst.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AshenDuel/ADGameplayTag/GameplayTags.h"
#include "AshenDuel/CoreFramework/Character/ADBossCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicsEngine/RadialForceComponent.h"

void UAN_Phase2_Burst::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                              const FAnimNotifyEventReference& EventReference)
{
	if (!MeshComp) return;
	
	AADBossCharacter* OwnerActor = Cast<AADBossCharacter>(MeshComp->GetOwner());
	UWorld* World = MeshComp->GetWorld();
	if (!OwnerActor || !World) return;
	
	if (CameraShake)
	{
		UGameplayStatics::PlayWorldCameraShake(
			World,
			CameraShake,
			OwnerActor->GetActorLocation(),
			0.f,
			1200.f);
	}
	
	URadialForceComponent* RadialForceComponent = OwnerActor->GetRadialForceComponent();
	
	if (RadialForceComponent)
	{
		RadialForceComponent->FireImpulse();
	}
	
	ACharacter* PlayerChar = UGameplayStatics::GetPlayerCharacter(World, 0);
	if (PlayerChar)
	{
		UAbilitySystemComponent* PlayerASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(PlayerChar);
		FVector BossLoc = OwnerActor->GetActorLocation();
		FVector PlayerLoc = PlayerChar->GetActorLocation();
		
		float Distance = FVector::Distance(BossLoc, PlayerLoc);
       
		float KnockbackRadius = 600.f; 
		if (Distance <= KnockbackRadius)
		{
			FVector LaunchDirection = (PlayerLoc - BossLoc).GetSafeNormal2D();
			FVector LaunchVelocity = LaunchDirection * 7000.f;
			PlayerChar->LaunchCharacter(LaunchVelocity, true, true);
		}
		
		if (PlayerASC)
		{
			FGameplayEventData Payload;
			Payload.EventTag = GameplayTags::Event::Character::Launch;
			PlayerASC->HandleGameplayEvent(GameplayTags::Event::Character::Launch, &Payload);
		}
	}
}
