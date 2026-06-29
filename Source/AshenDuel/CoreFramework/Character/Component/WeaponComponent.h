// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponComponent.generated.h"


class UAbilitySystemComponent;
class UGameplayEffect;
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
	
#pragma region Attack
public:
	void BeginWeaponTrace(USkeletalMeshComponent* SkelMesh, FName StartSocket, FName EndSocket);
	void TickWeaponTrace(FName StartSocketName, FName EndSocketName, float TraceRad, bool bDrawDebug);

protected:
	void DebugAttackTrace(FVector CapsuleCenter, float CapsuleHalfHeight, float TraceRad, FQuat CapsuleRotation, bool bHit);
	void HitAttackTargetApplyGE(TArray<FHitResult>& OutHits);
private:
	UPROPERTY()
	TSet<TObjectPtr<AActor>> AlreadyApplyDamageActors;
	
	UPROPERTY()
	TObjectPtr<USkeletalMeshComponent> OwnerMesh;
	
	bool bIsFirstTick;
	FVector PreviousStartLocation;
	FVector PreviousEndLocation;
#pragma endregion
	
	
#pragma region GameplayEffect
protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> DamageEffectClass;
#pragma endregion
};

