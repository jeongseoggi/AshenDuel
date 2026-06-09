#include "SwordWeapon.h"


ASwordWeapon::ASwordWeapon()
{
	PrimaryActorTick.bCanEverTick = false;
	
	SceneComp = CreateDefaultSubobject<USceneComponent>("SceneComponent");
	SetRootComponent(SceneComp);
	
	SwordMesh = CreateDefaultSubobject<UStaticMeshComponent>("SwordMesh");
	SwordMesh->SetupAttachment(SceneComp);
}


void ASwordWeapon::BeginPlay()
{
	Super::BeginPlay();
}

