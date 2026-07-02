 // Fill out your copyright notice in the Description page of Project Settings.


#include "ADDataManagerSubSystem.h"

#include "ADProjectSettings.h"

 void UADDataManagerSubSystem::Initialize(FSubsystemCollectionBase& Collection)
 {
	 Super::Initialize(Collection);
 	
 	const UADProjectSettings* CombatSettings = GetDefault<UADProjectSettings>();
 	if (CombatSettings && CombatSettings->GlobalAttackDataAssetPath.IsValid())
 	{
 		InitializeDataManager(Cast<UAttackDataAsset>(CombatSettings->GlobalAttackDataAssetPath.TryLoad()));
 	}
 }

 void UADDataManagerSubSystem::InitializeDataManager(UAttackDataAsset* InAsset)
 {
 	if (InAsset)
 	{
 		PlayerAttackDataAsset = InAsset;
 		UE_LOG(LogTemp, Warning, TEXT("DataAsset 등록 완료"));
 	}
 }

 FComboAttackData UADDataManagerSubSystem::GetAttackDataByTag(const FGameplayTag& AttackTag) const
 {
 	return PlayerAttackDataAsset ? PlayerAttackDataAsset->GetComboData(AttackTag) : FComboAttackData();
 }
