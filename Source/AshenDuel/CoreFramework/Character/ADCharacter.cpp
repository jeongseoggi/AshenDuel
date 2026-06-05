#include "ADCharacter.h"

#include "AshenDuel/CoreFramework/ADPlayerState.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

AADCharacter::AADCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
}

void AADCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	ASC = GetAbilitySystemComponent();
}
void AADCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

UAbilitySystemComponent* AADCharacter::GetAbilitySystemComponent() const
{
	if (AADPlayerState* PS = Cast<AADPlayerState>(GetPlayerState()))
	{
		return PS->GetAbilitySystemComponent();
	}
	return nullptr;
}

