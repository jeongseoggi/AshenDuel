#pragma once

#include "NativeGameplayTags.h"

namespace GameplayTags
{
	namespace State
	{
		ASHENDUEL_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(LockOn);
		ASHENDUEL_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Sprinting);

		namespace Combat
		{
			ASHENDUEL_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(ComboWindow);
		}
	}
	
	namespace Input
	{
		ASHENDUEL_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Sprinting);
		ASHENDUEL_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attack);
	}
	
	namespace Event
	{
		namespace Combo
		{
			ASHENDUEL_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(WindowOpen);
			ASHENDUEL_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputPressed);
		}
	}
}
