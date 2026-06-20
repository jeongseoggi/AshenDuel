
#include "ReactorComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "AshenDuel/ADGameplayTag/GameplayTags.h"
#include "AshenDuel/CoreFramework/Character/ADCharacter.h"

UReactorComponent::UReactorComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UReactorComponent::PlayHitReaction(const FVector& DamageCauserLocation)
{
	ACharacter* OwnerChar = Cast<ACharacter>(GetOwner());
	if (!OwnerChar) return;
	
	// 방향 벡터 계산
	FVector OwnerForward = OwnerChar->GetActorForwardVector().GetSafeNormal2D();
	FVector OwnerRight = OwnerChar->GetActorRightVector().GetSafeNormal2D();
	
	// 오너 위치에서 공격자 위치를 바라보는 방향 벡터
	FVector TargetDir = (DamageCauserLocation - OwnerChar->GetActorLocation()).GetSafeNormal2D();
	
	float DotForward = FVector::DotProduct(OwnerForward, TargetDir);
	float DotRight = FVector::DotProduct(OwnerRight, TargetDir);
	
	FGameplayTag DirectionTag;
	
	if (DotForward > 0.70f) DirectionTag = GameplayTags::HitReact::Front;
	else if (DotForward < -0.70f) DirectionTag = GameplayTags::HitReact::Back;
	else if (DotRight > 0.0f) DirectionTag = GameplayTags::HitReact::Right;
	else DirectionTag = GameplayTags::HitReact::Left;
	
	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner());
	if (ASC)
	{
		FGameplayEventData Payload;
		Payload.Instigator = OwnerChar;
		Payload.TargetTags.AddTag(DirectionTag);
		
		ASC->HandleGameplayEvent(GameplayTags::Event::HitReact, &Payload);
	}
}



