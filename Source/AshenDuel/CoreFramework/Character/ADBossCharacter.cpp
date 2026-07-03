#include "ADBossCharacter.h"

#include "AbilitySystemComponent.h"
#include "MotionWarpingComponent.h"
#include "AshenDuel/AshenDuel.h"
#include "AshenDuel/CoreFramework/ADPlayerController.h"
#include "AshenDuel/GAS/AttributeSet/ADBossAttributeSet.h"
#include "Component/FatalZoneComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"


AADBossCharacter::AADBossCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	
	TargetingSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("TargetingSphere"));
	TargetingSphereComponent->SetupAttachment(GetRootComponent());
	TargetingSphereComponent->SetCollisionObjectType(COLLISION_OBJECT_TARGETING);
	TargetingSphereComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	TargetingSphereComponent->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	
	FrontFatalZone = CreateDefaultSubobject<UFatalZoneComponent>(TEXT("FrontFatalZone"));
	BackFatalZone = CreateDefaultSubobject<UFatalZoneComponent>(TEXT("BackFatalZone"));
	
	FrontFatalZone->SetupAttachment(GetRootComponent());
	BackFatalZone->SetupAttachment(GetRootComponent());
	
	FrontFatalZone->bIsFrontZone = true;
	BackFatalZone->bIsFrontZone = false;
	
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
	BossAttributeSet = CreateDefaultSubobject<UADBossAttributeSet>(TEXT("BossAttributeSet"));
	MotionWarpingComponent = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarpingComponent"));
}

void AADBossCharacter::SetBossDeath()
{
	bIsDeath = !bIsDeath;
}

void AADBossCharacter::ApplyGroggy()
{
	FActiveGameplayEffectHandle ActiveGameplayEffect = ApplyGameplayEffectToSelf(GroggyEffectClass);
	if (ActiveGameplayEffect.IsValid())
	{
		FActiveGameplayEffectEvents* EffectEvents = ASC->GetActiveEffectEventSet(ActiveGameplayEffect);
		EffectEvents->OnEffectRemoved.AddUObject(this, &ThisClass::OnGameplayEffectRemoved);
	}
	
	UE_LOG(LogTemp, Warning, TEXT("ApplyGroggy"));
}

void AADBossCharacter::SetFatalZoneEnabled(bool bEnable)
{
	ECollisionEnabled::Type NewCollisionState = bEnable ? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision;

	if (FrontFatalZone)
	{
		FrontFatalZone->SetZoneEnabled(bEnable);
	}
	if (BackFatalZone)
	{
		BackFatalZone->SetZoneEnabled(bEnable);
	}

	UE_LOG(LogTemp, Log, TEXT("보스 페이탈 존 활성화 상태 변경: %s"), bEnable ? TEXT("True") : TEXT("False"));
}

void AADBossCharacter::BeginPlay()
{
	Super::BeginPlay();
	ASC = GetAbilitySystemComponent();
	
	if (ASC)
	{
		ASC->InitAbilityActorInfo(this, this);
		GiveDefaultAbilities();
		ShowBossUI();
	}
}

void AADBossCharacter::OnGameplayEffectRemoved(const FGameplayEffectRemovalInfo& EffectRemoved)
{
	ResetGroggy();
}

void AADBossCharacter::ResetGroggy()
{
	if (ASC)
	{
		float MaxGroggy = ASC->GetNumericAttribute(UADBossAttributeSet::GetMaxGroggyGaugeAttribute());
		ASC->SetNumericAttributeBase(UADBossAttributeSet::GetGroggyGaugeAttribute(), MaxGroggy);
		UE_LOG(LogTemp, Log, TEXT("보스 그로기 게이지 리셋 : %f"), MaxGroggy);
		
	}
}

void AADBossCharacter::OnTargeted(bool bTargeted)
{
	
}

bool AADBossCharacter::CanBeTargeted()
{
	if (bIsDeath)
	{
		return false;
	}
	return true;
}

void AADBossCharacter::ShowBossUI()
{
	AADPlayerController* PC = Cast<AADPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PC)
	{
		PC->RegisterBossToHUD(this);
	}
}

void AADBossCharacter::InitiateDeathRow()
{
	if (OnBossDied.IsBound())
	{
		OnBossDied.Broadcast();
	}
}


