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
			.AddUObject(this, &AADAIController::UpdateAIState);
			
			ASC->RegisterGameplayTagEvent(GameplayTags::State::Boss::Phase2Change, EGameplayTagEventType::NewOrRemoved)
			.AddUObject(this, &AADAIController::UpdateAIState);
			
			ASC->RegisterGameplayTagEvent(GameplayTags::State::Boss::IsGettingUp, EGameplayTagEventType::NewOrRemoved)
			.AddUObject(this, &AADAIController::UpdateAIState);
		}
	}
}

void AADAIController::UpdateAIState(const FGameplayTag Tag, int32 NewCount)
{
	if (BlackboardComp && GetPawn())
	{
		if (AADBossCharacter* BossChar = Cast<AADBossCharacter>(GetPawn()))
		{
			UAbilitySystemComponent* ASC = BossChar->GetAbilitySystemComponent();
			if (ASC)
			{
				bool bIsKnockDown = ASC->HasMatchingGameplayTag(GameplayTags::State::KnockDown);
				bool bIsPhase2 = ASC->HasMatchingGameplayTag(GameplayTags::State::Boss::Phase2Change);
				bool bIsGettingUp = ASC->HasMatchingGameplayTag(GameplayTags::State::Boss::IsGettingUp);
                
				bool bShouldDisableAI = bIsKnockDown || bIsPhase2 || bIsGettingUp;
				
				if (bShouldDisableAI)
				{
					ClearFocus(EAIFocusPriority::Gameplay);
				}
				else
				{
					SetFocus(PlayerChar);
				}
				
				Blackboard->SetValueAsBool(TEXT("bIsAIDisabled"), bShouldDisableAI);
			}
		}
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

