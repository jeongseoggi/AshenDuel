// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SwordWeapon.generated.h"

class UNiagaraComponent;
class UNiagaraSystem;

UCLASS()
class ASHENDUEL_API ASwordWeapon : public AActor
{
	GENERATED_BODY()

public:
	ASwordWeapon();
	
	FORCEINLINE UStaticMeshComponent* GetMesh() const { return SwordMesh; }
	
	void ActivateTrail(UNiagaraSystem* InTrailSystem, FName InSocketName);
	void DeactivateTrail();

protected:
	virtual void BeginPlay() override;
	
protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USceneComponent> SceneComp;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UStaticMeshComponent> SwordMesh;
	
	UPROPERTY()
	TObjectPtr<UNiagaraComponent> ActiveTrailComp;
};
