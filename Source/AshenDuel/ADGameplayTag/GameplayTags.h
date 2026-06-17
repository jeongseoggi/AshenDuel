#pragma once

#include "NativeGameplayTags.h"

namespace GameplayTags
{
	namespace State
	{
		ASHENDUEL_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(LockOn);
		ASHENDUEL_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Sprinting);
		ASHENDUEL_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Dodge);

		namespace Combat
		{
			ASHENDUEL_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(ComboWindow);
		}
		
		namespace Stamina
		{
			ASHENDUEL_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(RegenBlocked);
		}
	}
	
	namespace Input
	{
		ASHENDUEL_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Sprinting);
		ASHENDUEL_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attack);
		ASHENDUEL_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Dodge);
	}
	
	namespace Event
	{
		ASHENDUEL_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Dodge_End);
		namespace Combo
		{
			ASHENDUEL_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(WindowOpen);
			ASHENDUEL_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputPressed);
		}
	}

	namespace Data
	{
		namespace Cost
		{
			ASHENDUEL_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Stamina);
		}
	}
}
