// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Actor.h"
#include "GCN_HealEffect.generated.h"

class UNiagaraComponent;
class UNiagaraSystem;

UCLASS()
class ASHENDUEL_API AGCN_HealEffect : public AGameplayCueNotify_Actor
{
	GENERATED_BODY()

public:
	AGCN_HealEffect();
	
	virtual bool OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) override;
	
	virtual bool OnRemove_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "VFX")
	TObjectPtr<UNiagaraSystem> HealEffectSystem;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "VFX")
	FName AttachSocketName;
	
private:
	UPROPERTY()
	TObjectPtr<UNiagaraComponent> SpawnedEffectComp;
};
