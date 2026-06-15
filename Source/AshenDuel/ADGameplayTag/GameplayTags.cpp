#include "GameplayTags.h"

namespace GameplayTags
{
	namespace State
	{
		ASHENDUEL_API UE_DEFINE_GAMEPLAY_TAG(LockOn, "State.LockOn");
		ASHENDUEL_API UE_DEFINE_GAMEPLAY_TAG(Sprinting, "State.Sprinting");

		namespace Combat
		{
			ASHENDUEL_API UE_DEFINE_GAMEPLAY_TAG(ComboWindow, "State.Combat.ComboWindow");
		}
	}
	
	namespace Input
	{
		ASHENDUEL_API UE_DEFINE_GAMEPLAY_TAG(Sprinting, "Input.Sprinting");
		ASHENDUEL_API UE_DEFINE_GAMEPLAY_TAG(Attack, "Input.Attack");
	}
	
	namespace Event
	{
		namespace Combo
		{
			ASHENDUEL_API UE_DEFINE_GAMEPLAY_TAG(WindowOpen, "Event.Combo.WindowOpen");
			ASHENDUEL_API UE_DEFINE_GAMEPLAY_TAG(InputPressed, "Event.Combo.InputPressed");
		}
	}
}
