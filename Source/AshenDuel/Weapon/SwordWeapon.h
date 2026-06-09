// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SwordWeapon.generated.h"

UCLASS()
class ASHENDUEL_API ASwordWeapon : public AActor
{
	GENERATED_BODY()

public:
	ASwordWeapon();

protected:
	virtual void BeginPlay() override;
	
protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USceneComponent> SceneComp;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UStaticMeshComponent> SwordMesh;
};
