#include "ADCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "AshenDuel/ADGameplayTag/GameplayTags.h"
#include "AshenDuel/CoreFramework/ADPlayerState.h"
#include "AshenDuel/GAS/Component/ADAbilitySystemComponent.h"
#include "AshenDuel/Input/ADEnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "Component/LockOnComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

AADCharacter::AADCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
	
	LockOnComponent = CreateDefaultSubobject<ULockOnComponent>(TEXT("LockOnComponent"));
}

void AADCharacter::BeginPlay()
{
	Super::BeginPlay();
	SetLockOnState(false);
}

void AADCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	ASC = GetAbilitySystemComponent();
	if (ASC)
	{
		ASC->InitAbilityActorInfo(this, this);
		GiveDefaultAbilities();
		ApplyStartUpEffects();
	}
}

void AADCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	APlayerController* PC = Cast<APlayerController>(GetController());
	check(PC);
	
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
	{
		if (DefaultMappingContext)
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AADCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AADCharacter::Look);
		EnhancedInputComponent->BindAction(LockOnAction, ETriggerEvent::Started, this, &AADCharacter::LockOn);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &AADCharacter::Input_Attack);
	}
	
	UADEnhancedInputComponent* ADInputComp = CastChecked<UADEnhancedInputComponent>(PlayerInputComponent);
	
	TArray<uint32> BindHandles;
	ADInputComp->BindAbilityActions(
		InputData,
		this,
		&ThisClass::Input_AbilityInputTagPressed,
		&ThisClass::Input_AbilityInputTagReleased,
		BindHandles
	);
}

UAbilitySystemComponent* AADCharacter::GetAbilitySystemComponent() const
{
	if (AADPlayerState* PS = Cast<AADPlayerState>(GetPlayerState()))
	{
		return PS->GetAbilitySystemComponent();
	}
	return Super::GetAbilitySystemComponent();
}

void AADCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(ForwardDirection, MovementVector.Y);
		
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AADCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookVec = Value.Get<FVector2D>();
	
	if (Controller)
	{
		AddControllerYawInput(LookVec.X);
		AddControllerPitchInput(-LookVec.Y);
	}
}

void AADCharacter::LockOn(const FInputActionValue& Value)
{
	LockOnComponent->ToggleLockOn();
}

void AADCharacter::Input_Attack(const FInputActionValue& Value)
{
	if (ASC)
	{
		FGameplayEventData Payload;
		Payload.EventTag = GameplayTags::Event::Combo::InputPressed;
		Payload.Instigator = this;
		Payload.Target = this;

		ASC->HandleGameplayEvent(GameplayTags::Event::Combo::InputPressed, &Payload);
	}
}

void AADCharacter::Input_AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (UADAbilitySystemComponent* ADASC = Cast<UADAbilitySystemComponent>(GetAbilitySystemComponent()))
	{
		ADASC->AbilityInputTagPressed(InputTag);
	}
}

void AADCharacter::Input_AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (UADAbilitySystemComponent* ADASC = Cast<UADAbilitySystemComponent>(GetAbilitySystemComponent()))
	{
		ADASC->AbilityInputTagReleased(InputTag);
	}
}

void AADCharacter::SetLockOnState(bool LockOnState)
{
	bUseControllerRotationYaw = LockOnState;
	GetCharacterMovement()->bOrientRotationToMovement = !LockOnState;
	
	if (SpringArm)
	{
		SpringArm->TargetArmLength = LockOnState ? 450.f : 300.f;
	}
}
