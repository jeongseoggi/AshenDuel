#include "ADCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "AshenDuel/CoreFramework/ADPlayerState.h"
#include "AshenDuel/GAS/Ability/ADGameplayAbility.h"
#include "AshenDuel/GAS/Component/ADAbilitySystemComponent.h"
#include "AshenDuel/Input/ADEnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "Component/LockOnComponent.h"
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
	
	ASC = GetAbilitySystemComponent();
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
	return nullptr;
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

void AADCharacter::GiveDefaultAbilities()
{
	check(ASC);
	
	for (TSubclassOf<UGameplayAbility> AbilityClass : DefaultAbilities)
	{
		if (ASC->FindAbilitySpecFromClass(AbilityClass))
		{
			continue;
		}
		
		FGameplayAbilitySpec Spec(AbilityClass, 1, INDEX_NONE, this);
		
		if (const UADGameplayAbility* ADAbility = Cast<UADGameplayAbility>(AbilityClass->GetDefaultObject()))
		{
			if (ADAbility->StartupInputTag.IsValid())
			{
				Spec.DynamicAbilityTags.AddTag(ADAbility->StartupInputTag);
			}
		}
		
		ASC->GiveAbility(Spec);
	}
}

