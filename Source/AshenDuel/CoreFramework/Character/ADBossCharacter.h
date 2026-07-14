// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ADCharacterBase.h"
#include "AshenDuel/Interface/EnemyHelper.h"
#include "ADBossCharacter.generated.h"

class URadialForceComponent;
class UFatalZoneComponent;
class UBoxComponent;
class UADAttributeSet;
class USphereComponent;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBossBossDiedDelegate);


UCLASS()
class ASHENDUEL_API AADBossCharacter : public AADCharacterBase, public IEnemyHelper
{
	GENERATED_BODY()

public:
	AADBossCharacter();
	
	void SetBossDeath();
	
	void ApplyGroggy();
	
	void SetFatalZoneEnabled(bool bEnable);
	
	FVector GetFatalZoneLocation();
	
	FRotator GetFatalZoneRotation();
	
	FORCEINLINE URadialForceComponent* GetRadialForceComponent() const { return RadialForceComponent; }
	FORCEINLINE USphereComponent* GetTargetingSphereComponent() const { return TargetingSphereComponent; }
	
protected:
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void OnGameplayEffectRemoved(const FGameplayEffectRemovalInfo& EffectRemoved);
	
	void ResetGroggy();
protected:
	UPROPERTY(EditDefaultsOnly)
	bool bIsDeath = false;
	
#pragma region Component
protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USphereComponent> TargetingSphereComponent;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UFatalZoneComponent> FrontFatalZone;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UFatalZoneComponent> BackFatalZone;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<URadialForceComponent> RadialForceComponent;
	
#pragma endregion

#pragma region EnemyHelperInterface
	virtual void OnTargeted(bool bTargeted) override;
	virtual bool CanBeTargeted() override;
#pragma endregion
	
#pragma region ASC
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UADAttributeSet> BossAttributeSet;
	
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	TSubclassOf<UGameplayEffect> GroggyEffectClass;
#pragma endregion
	
#pragma region UI
protected:
	void ShowBossUI();
#pragma endregion
	
#pragma region Delegate
public:
	UPROPERTY(BlueprintAssignable, Category = "DelegateEvents")
	FOnBossBossDiedDelegate OnBossDied;
	
	void InitiateDeathRow();

#pragma endregion
	
#pragma region PhaseVar
public:
	FORCEINLINE bool GetPhase2Triggered() const {return bIsPhase2Triggered;}
	FORCEINLINE void SetPhase2Triggered(bool bTriggered) {bIsPhase2Triggered = bTriggered;}
	
protected:
	bool bIsPhase2Triggered = false;
#pragma endregion
};
