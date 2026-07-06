// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "FatalZoneComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ASHENDUEL_API UFatalZoneComponent : public UBoxComponent
{
	GENERATED_BODY()

public:
	UFatalZoneComponent();
	void SetZoneEnabled(bool Enable);
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	FORCEINLINE bool GetFatalZoneEnabled() const { return bIsFatalZoneActive; }
	
protected:
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void OnZoneOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, 
							UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
							bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnZoneOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, 
										   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FatalZone")
	bool bIsFrontZone = true;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FatalZone")
	bool bIsFatalZoneActive;
};
