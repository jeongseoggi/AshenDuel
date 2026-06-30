#include "GameplayTags.h"

namespace GameplayTags
{
	namespace State
	{
		ASHENDUEL_API UE_DEFINE_GAMEPLAY_TAG(LockOn, "State.LockOn");
		ASHENDUEL_API UE_DEFINE_GAMEPLAY_TAG(Sprinting, "State.Sprinting");
		ASHENDUEL_API UE_DEFINE_GAMEPLAY_TAG(Dodge, "State.Dodge");
		ASHENDUEL_API UE_DEFINE_GAMEPLAY_TAG(Invincible, "State.Invincible");
		ASHENDUEL_API UE_DEFINE_GAMEPLAY_TAG(Death, "State.Death");
		ASHENDUEL_API UE_DEFINE_GAMEPLAY_TAG(IsGoingToDie, "State.IsGoingToDie");

		namespace Combat
		{
			ASHENDUEL_API UE_DEFINE_GAMEPLAY_TAG(ComboWindow, "State.Combat.ComboWindow");
		}

		namespace Stamina
		{
			ASHENDUEL_API UE_DEFINE_GAMEPLAY_TAG(RegenBlocked, "State.Stamina.RegenBlocked");
		}
		
		namespace Setup
		{
			ASHENDUEL_API UE_DEFINE_GAMEPLAY_TAG(Completed, "State.Setup.Completed");
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
		ASHENDUEL_API UE_DEFINE_GAMEPLAY_TAG(Death, "Event.Death");
		ASHENDUEL_API UE_DEFINE_GAMEPLAY_TAG(Boss_Death, "Event.Boss_Death");
		namespace Combo
		{
			ASHENDUEL_API UE_DEFINE_GAMEPLAY_TAG(WindowOpen, "Event.Combo.WindowOpen");
			ASHENDUEL_API UE_DEFINE_GAMEPLAY_TAG(InputPressed, "Event.Combo.InputPressed");
		}
		
		namespace Character
		{
			ASHENDUEL_API UE_DEFINE_GAMEPLAY_TAG(Ragdoll, "Event.Character.Ragdoll");
		}

		namespace Boss
		{
			ASHENDUEL_API UE_DEFINE_GAMEPLAY_TAG(DeathCompleted, "Event.Boss.DeathCompleted");
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

	namespace Ability
	{
		namespace Action
		{
			ASHENDUEL_API UE_DEFINE_GAMEPLAY_TAG(Dodge, "Ability.Action.Dodge");
			ASHENDUEL_API UE_DEFINE_GAMEPLAY_TAG(Attack, "Ability.Action.Attack");
		}
		
		namespace Boss
		{
			ASHENDUEL_API UE_DEFINE_GAMEPLAY_TAG(BasicAttack, "Ability.Boss.BasicAttack");
		}
	}
	
	namespace GameplayEvent
	{
		ASHENDUEL_API UE_DEFINE_GAMEPLAY_TAG(HitReact, "GameplayEvent.HitReact");
	}
}
