#include "GameplayTags.h"

namespace GameplayTags
{
	namespace State
	{
		ASHENDUEL_API UE_DEFINE_GAMEPLAY_TAG(LockOn, "State.LockOn");
		ASHENDUEL_API UE_DEFINE_GAMEPLAY_TAG(Sprinting, "State.Sprinting");
		ASHENDUEL_API UE_DEFINE_GAMEPLAY_TAG(Dodge, "State.Dodge");
		ASHENDUEL_API UE_DEFINE_GAMEPLAY_TAG(Invincible, "State.Invincible");

		namespace Combat
		{
			ASHENDUEL_API UE_DEFINE_GAMEPLAY_TAG(ComboWindow, "State.Combat.ComboWindow");
		}

		namespace Stamina
		{
			ASHENDUEL_API UE_DEFINE_GAMEPLAY_TAG(RegenBlocked, "State.Stamina.RegenBlocked");
		}
	}
	
	namespace Input
	{
		ASHENDUEL_API UE_DEFINE_GAMEPLAY_TAG(Sprinting, "Input.Sprinting");
		ASHENDUEL_API UE_DEFINE_GAMEPLAY_TAG(Attack, "Input.Attack");
		ASHENDUEL_API UE_DEFINE_GAMEPLAY_TAG(Dodge, "Input.Dodge");
	}
	
	namespace Event
	{
		ASHENDUEL_API UE_DEFINE_GAMEPLAY_TAG(Dodge_End, "Event.Dodge_End");
		ASHENDUEL_API UE_DEFINE_GAMEPLAY_TAG(HitReact, "Event.HitReact");
		namespace Combo
		{
			ASHENDUEL_API UE_DEFINE_GAMEPLAY_TAG(WindowOpen, "Event.Combo.WindowOpen");
			ASHENDUEL_API UE_DEFINE_GAMEPLAY_TAG(InputPressed, "Event.Combo.InputPressed");
		}
	}
	
	namespace Data
	{
		namespace Cost
		{
			ASHENDUEL_API UE_DEFINE_GAMEPLAY_TAG(Stamina, "Data.Cost.Stamina");
		}
	}

	namespace HitReact
	{
		ASHENDUEL_API UE_DEFINE_GAMEPLAY_TAG(Front, "HitReact.Front");
		ASHENDUEL_API UE_DEFINE_GAMEPLAY_TAG(Back, "HitReact.Back");
		ASHENDUEL_API UE_DEFINE_GAMEPLAY_TAG(Right, "HitReact.Right");
		ASHENDUEL_API UE_DEFINE_GAMEPLAY_TAG(Left, "HitReact.Left");
	}
}
