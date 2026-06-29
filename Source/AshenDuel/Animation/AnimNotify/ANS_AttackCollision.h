// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "ANS_AttackCollision.generated.h"

class UWeaponComponent;
/**
 * 
 */
UCLASS()
class ASHENDUEL_API UANS_AttackCollision : public UAnimNotifyState
{
	GENERATED_BODY()
	
public:
	UANS_AttackCollision();
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	
protected:
	UPROPERTY(EditAnywhere, Category = "Collision")
	FName StartSocketName;
	
	UPROPERTY(EditAnywhere, Category = "Collision")
	FName EndSocketName;
	
	UPROPERTY(EditAnywhere, Category = "Collision")
	float TraceRadius;
	
	UPROPERTY(EditAnywhere, Category = "Collision")
	bool bDrawDebug;
};