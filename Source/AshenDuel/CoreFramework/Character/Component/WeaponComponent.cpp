#include "WeaponComponent.h"

#include "AshenDuel/CoreFramework/Character/ADCharacter.h"
#include "AshenDuel/Weapon/SwordWeapon.h"

UWeaponComponent::UWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UWeaponComponent::BeginPlay()
{
	Super::BeginPlay();
	SpawnAndAttachWeapon();
}

void UWeaponComponent::SpawnAndAttachWeapon()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = GetOwner();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	
	if (!WeaponActorClass) return;
	
	WeaponActorInst = GetWorld()->SpawnActor<ASwordWeapon>(WeaponActorClass, SpawnParams);
	
	AADCharacter* OwnerCharacter = Cast<AADCharacter>(GetOwner());
	checkf(OwnerCharacter, TEXT("Cast Failed Character"));
	
	WeaponActorInst->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, SocketName);
}



