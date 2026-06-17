// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ADInputData.h"
#include "EnhancedInputComponent.h"
#include "ADEnhancedInputComponent.generated.h"



UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ASHENDUEL_API UADEnhancedInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:
	template<class UserClass, typename PressedFuncType, typename ReleasedFuncType>
		void BindAbilityActions(const UADInputData* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, TArray<uint32>& BindHandles);

};

template <class UserClass, typename PressedFuncType, typename ReleasedFuncType>
void UADEnhancedInputComponent::BindAbilityActions(const UADInputData* InputConfig, UserClass* Object,
	PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, TArray<uint32>& BindHandles)
{
	if (!InputConfig) return;
	
	for (const FADInputAction& Action : InputConfig->AbilityActions)
	{
		if (!Action.Action || !Action.InputTag.IsValid())
		{
			continue;
		}
		
		if (PressedFunc && Action.bBindPressed)
		{
        	BindAction(
        		Action.Action,
        		Action.PressedTriggerEvent,
        		Object,
        		PressedFunc,
        		Action.InputTag
        	);
		}
        
        if (ReleasedFunc && Action.bBindReleased)
        {
        	BindAction(
        		Action.Action,
        		Action.ReleasedTriggerEvent,
        		Object,
        		ReleasedFunc,
        		Action.InputTag
        	);
        }
	}
}

