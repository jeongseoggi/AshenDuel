#include "ADAIController.h"

#include "AbilitySystemComponent.h"
#include "AshenDuel/ADGameplayTag/GameplayTags.h"
#include "AshenDuel/CoreFramework/Character/ADBossCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

AADAIController::AADAIController()
{
	PrimaryActorTick.bCanEverTick = true;
	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard"));
	bAllowStrafe = true;
}

EBossAttackDirection AADAIController::CheckPlayerLocation()
{
	AADBossCharacter* BossChar = Cast<AADBossCharacter>(GetPawn());
	if (!PlayerChar|| !BossChar) return EBossAttackDirection::Left;
	
	FVector BossForwad = BossChar->GetActorForwardVector();
	BossForwad.Z = 0.0f;
	BossForwad.Normalize();
	
	FVector DirectionToPlayer = PlayerChar->GetActorLocation() - BossChar->GetActorLocation();
	DirectionToPlayer.Z = 0.0f;
	DirectionToPlayer.Normalize();
	
	FVector CrossProduct = FVector::CrossProduct(BossForwad, DirectionToPlayer);
	
	if (CrossProduct.Z > 0.1f) return EBossAttackDirection::Left;
	else if (CrossProduct.Z < -0.1f) return EBossAttackDirection::Right;
	
	
	return EBossAttackDirection::Left;
}

AActor* AADAIController::GetTargetActor()
{
	return PlayerChar;
}

void AADAIController::BeginPlay()
{
	Super::BeginPlay();
	
}

void AADAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	if (BBData && BlackboardComp)
	{
		UBlackboardComponent* BBCompRaw = BlackboardComp.Get();
		if (UseBlackboard(BBData, BBCompRaw))
		{
			if (BTAsset)
			{
				bool bRunSuccess = RunBehaviorTree(BTAsset);
				if (!bRunSuccess)
				{
					UE_LOG(LogTemp, Error, TEXT("AADAIController: RunBehaviorTree Failed!"));
				}
			}
		}
	}
	
	if (AADBossCharacter* BossChar = Cast<AADBossCharacter>(InPawn))
	{
		UAbilitySystemComponent* ASC = BossChar->GetAbilitySystemComponent();
		if (ASC && BlackboardComp)
		{
			ASC->RegisterGameplayTagEvent(GameplayTags::State::KnockDown, EGameplayTagEventType::NewOrRemoved)
			.AddUObject(this, &AADAIController::OnKnockDownTagChanged);
		}
	}
}

void AADAIController::OnKnockDownTagChanged(const FGameplayTag Tag, int32 NewCount)
{
	if (BlackboardComp)
	{
		bool bIsKnockDown = (NewCount > 0);
		Blackboard->SetValueAsBool(TEXT("bIsKnockDown"), bIsKnockDown);
        
		UE_LOG(LogTemp, Log, TEXT("BT 블랙보드 싱크 - bIsKnockDown 변경: %s"), bIsKnockDown ? TEXT("True") : TEXT("False"));
	}
}

void AADAIController::InitializeTargetActor()
{
	PlayerChar = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	
	if (PlayerChar && BlackboardComp)
	{
		BlackboardComp->SetValueAsObject(TEXT("TargetActor"), PlayerChar);
		SetFocus(PlayerChar);
	}
}

void AADAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (BlackboardComp) return;
	
	if (BlackboardComp->GetValueAsBool(TEXT("bIsChasing")))
	{
		if (PlayerChar) SetFocus(PlayerChar);
	}
	else
	{
		if (PlayerChar) ClearFocus(EAIFocusPriority::Gameplay);
	}
}

