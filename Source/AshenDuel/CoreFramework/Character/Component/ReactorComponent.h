// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ReactorComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ASHENDUEL_API UReactorComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UReactorComponent();
	void PlayHitReaction(const FVector& DamageCauserLocation);
};
