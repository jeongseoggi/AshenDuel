#include "SwordWeapon.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"


ASwordWeapon::ASwordWeapon()
{
	PrimaryActorTick.bCanEverTick = false;
	
	SceneComp = CreateDefaultSubobject<USceneComponent>("SceneComponent");
	SetRootComponent(SceneComp);
	
	SwordMesh = CreateDefaultSubobject<UStaticMeshComponent>("SwordMesh");
	SwordMesh->SetupAttachment(SceneComp);
}

void ASwordWeapon::ActivateTrail(UNiagaraSystem* InTrailSystem, FName InSocketName)
{
	if (!InTrailSystem || !GetMesh()) return;
	
	DeactivateTrail();
	
	ActiveTrailComp = UNiagaraFunctionLibrary::SpawnSystemAttached(
		InTrailSystem,
		SwordMesh,
		InSocketName,
		FVector::ZeroVector,
		FRotator::ZeroRotator,
		EAttachLocation::SnapToTargetIncludingScale,
		true
	);
}

void ASwordWeapon::DeactivateTrail()
{
	if (ActiveTrailComp && ActiveTrailComp->IsActive())
	{
		ActiveTrailComp->Deactivate();
		ActiveTrailComp = nullptr;
	}
}


void ASwordWeapon::BeginPlay()
{
	Super::BeginPlay();
}

