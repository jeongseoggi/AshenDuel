// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponComponent.generated.h"


class ASwordWeapon;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ASHENDUEL_API UWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UWeaponComponent();

protected:
	virtual void BeginPlay() override;
	void SpawnAndAttachWeapon();
	
protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ASwordWeapon> WeaponActorClass;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<ASwordWeapon> WeaponActorInst;
	
	UPROPERTY(EditDefaultsOnly)
	FName SocketName = FName(TEXT("WeaponSocket"));
};

