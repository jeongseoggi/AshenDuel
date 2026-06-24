#include "ADBossCharacter.h"

#include "AbilitySystemComponent.h"
#include "MotionWarpingComponent.h"
#include "AshenDuel/AshenDuel.h"
#include "AshenDuel/GAS/AttributeSet/ADBossAttributeSet.h"
#include "Components/SphereComponent.h"


AADBossCharacter::AADBossCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	
	TargetingSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("TargetingSphere"));
	TargetingSphereComponent->SetupAttachment(GetRootComponent());
	TargetingSphereComponent->SetCollisionObjectType(COLLISION_OBJECT_TARGETING);
	TargetingSphereComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	TargetingSphereComponent->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
	BossAttributeSet = CreateDefaultSubobject<UADBossAttributeSet>(TEXT("BossAttributeSet"));
	MotionWarpingComponent = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarpingComponent"));
}

void AADBossCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (ASC)
	{
		ASC->InitAbilityActorInfo(this, this);
		GiveDefaultAbilities();
	}
}

void AADBossCharacter::OnTargeted(bool bTargeted)
{
	
}

bool AADBossCharacter::CanBeTargeted()
{
	return true;
}


