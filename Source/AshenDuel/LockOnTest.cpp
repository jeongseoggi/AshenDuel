
#include "LockOnTest.h"

#include "AshenDuel.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"


ALockOnTest::ALockOnTest()
{
	PrimaryActorTick.bCanEverTick = false;
	
	TargetingSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("TargetingSphere"));
	TargetingSphereComponent->SetupAttachment(GetRootComponent());
	TargetingSphereComponent->SetCollisionObjectType(COLLISION_OBJECT_TARGETING);
	TargetingSphereComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	TargetingSphereComponent->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
}

void ALockOnTest::BeginPlay()
{
	Super::BeginPlay();
	
}

void ALockOnTest::OnTargeted(bool bTargeted)
{
	
}

bool ALockOnTest::CanBeTargeted()
{
	return true;
}

