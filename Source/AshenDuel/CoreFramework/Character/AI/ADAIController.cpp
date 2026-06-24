#include "ADAIController.h"

#include "AshenDuel/CoreFramework/Character/ADBossCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

AADAIController::AADAIController()
{
	PrimaryActorTick.bCanEverTick = false;
	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard"));
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
	
	UE_LOG(LogTemp, Warning, TEXT("%f"), CrossProduct.Z);
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
			
			FTimerHandle TargetInitTimerHandle;
			GetWorldTimerManager().SetTimer(TargetInitTimerHandle, this, &AADAIController::InitializeTargetActor, 0.1f, false);
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

