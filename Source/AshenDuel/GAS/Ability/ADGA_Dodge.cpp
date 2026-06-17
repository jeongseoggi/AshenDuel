// Fill out your copyright notice in the Description page of Project Settings.


#include "ADGA_Dodge.h"

#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "AshenDuel/ADGameplayTag/GameplayTags.h"
#include "AshenDuel/Animation/ADAnimInstance.h"
#include "GameFramework/Character.h"


void UADGA_Dodge::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                  const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
    
	ACharacter* OwnerChar = Cast<ACharacter>(GetAvatarActorFromActorInfo());
    if (!OwnerChar || !DodgeMontage) // DodgeMontage는 AM_Dodge 에셋 포인터입니다.
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
        return;
    }

    // 1. 캐릭터 방향 및 입력 방향 계산
    FVector Forward = OwnerChar->GetActorForwardVector();
    FVector InputDir = OwnerChar->GetLastMovementInputVector();

    int32 TargetIndex = 0;

    if (!InputDir.IsNearlyZero())
    {
        InputDir.Normalize();
        
        float CosAngle = FVector::DotProduct(Forward, InputDir);
        float Angle = FMath::RadiansToDegrees(FMath::Acos(CosAngle));
        
        FVector Cross = FVector::CrossProduct(Forward, InputDir);
        if (Cross.Z < 0.0f)
        {
            Angle = -Angle; 
        }
        
        if (Angle >= -22.5f && Angle < 22.5f)         TargetIndex = 0; // 전방 (Section0)
        else if (Angle >= 22.5f && Angle < 67.5f)     TargetIndex = 1; // 전우 (Section1)
        else if (Angle >= 67.5f && Angle < 112.5f)    TargetIndex = 2; // 우측 (Section2)
        else if (Angle >= 112.5f && Angle < 157.5f)   TargetIndex = 3; // 후우 (Section3)
        else if (Angle >= 157.5f || Angle < -157.5f)  TargetIndex = 4; // 후방 (Section4)
        else if (Angle >= -157.5f && Angle < -112.5f) TargetIndex = 5; // 후좌 (Section5)
        else if (Angle >= -112.5f && Angle < -67.5f)  TargetIndex = 6; // 좌측 (Section6)
        else if (Angle >= -67.5f && Angle < -22.5f)   TargetIndex = 7; // 전좌 (Section7)
    }
    
    FName TargetSectionName = FName(*FString::Printf(TEXT("Section%d"), TargetIndex));
    UAnimMontage* SelectedMontage = DodgeMontage;
    
    if (UADAnimInstance* AnimInst = Cast<UADAnimInstance>(OwnerChar->GetMesh()->GetAnimInstance()))
    {
        SelectedMontage = AnimInst->GetIsSprinting() ? SprintDodge : DodgeMontage;
    }
    
    
    UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
        this, NAME_None, SelectedMontage, 1.0f, TargetSectionName);
    
    if (SelectedMontage)
    {
        // 몽타주가 완전히 완료되거나 취소되었을 때 동일하게 종료 콜백 지정
        MontageTask->OnCompleted.AddDynamic(this, &UADGA_Dodge::OnDodgeMontageEnded);
        MontageTask->OnInterrupted.AddDynamic(this, &UADGA_Dodge::OnDodgeMontageEnded);
        MontageTask->ReadyForActivation();
    }
    else
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
    }
}

void UADGA_Dodge::OnDodgeMontageEnded()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UADGA_Dodge::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                             const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
