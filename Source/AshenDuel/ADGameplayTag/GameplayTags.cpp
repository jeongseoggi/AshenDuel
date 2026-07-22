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
		ASHENDUEL_API UE_DEFINE_GAMEPLAY_TAG(Block, "State.Block");
		ASHENDUEL_API UE_DEFINE_GAMEPLAY_TAG(IsParrying, "State.IsParrying");
		ASHENDUEL_API UE_DEFINE_GAMEPLAY_TAG(IsGuardBroken, "State.IsGuardBroken");
		ASHENDUEL_API UE_DEFINE_GAMEPLAY_TAG(IsHeavyAttack, "State.IsHeavyAttack");
		ASHENDUEL_API UE_DEFINE_GAMEPLAY_TAG(HeavyAttackCharageEnd, "State.HeavyAttackCharageEnd");
		ASHENDUEL_API UE_DEFINE_GAMEPLAY_TAG(IsGroggy, "State.IsGroggy");
		ASHENDUEL_API UE_DEFINE_GAMEPLAY_TAG(KnockDown, "State.KnockDown");
		ASHENDUEL_API UE_DEFINE_GAMEPLAY_TAG(CanFatal, "State.CanFatal");
		ASHENDUEL_API UE_DEFINE_GAMEPLAY_TAG(IsLockMovement, "State.IsLockMovement");
		ASHENDUEL_API UE_DEFINE_GAMEPLAY_TAG(Launch, "State.Launch");
		ASHENDUEL_API UE_DEFINE_GAMEPLAY_TAG(Hit, "State.Hit");
		ASHENDUEL_API UE_DEFINE_GAMEPLAY_TAG(UsingPotion, "State.UsingPotion");
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

		namespace Boss
		{
			ASHENDUEL_API UE_DEFINE_GAMEPLAY_TAG(Phase2Change, "State.Boss.Phase2Change");
			ASHENDUEL_API UE_DEFINE_GAMEPLAY_TAG(Phase2, "State.Boss.Phase2");
			ASHENDUEL_API UE_DEFINE_GAMEPLAY_TAG(IsBeingFatalAttacked, "State.IsBeingFatalAttacked");
			ASHENDUEL_API UE_DEFINE_GAMEPLAY_TAG(IsGettingUp, "State.IsGettingUp");
		}
	}
	
	namespace Input
	{
		ASHENDUEL_API UE_DEFINE_GAMEPLAY_TAG(Sprinting, "Input.Sprinting");
		ASHENDUEL_API UE_DEFINE_GAMEPLAY_TAG(Attack, "Input.Attack");
		ASHENDUEL_API UE_DEFINE_GAMEPLAY_TAG(Dodge, "Input.Dodge");
		ASHENDUEL_API UE_DEFINE_GAMEPLAY_TAG(Block, "Input.Block");
		ASHENDUEL_API UE_DEFINE_GAMEPLAY_TAG(HeavyAttack, "Input.HeavyAttack");
		ASHENDUEL_API UE_DEFINE_GAMEPLAY_TAG(FatalAttack, "Input.FatalAttack");
		ASHENDUEL_API UE_DEFINE_GAMEPLAY_TAG(UsingPotion, "Input.UsingPotion");
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
			ASHENDUEL_API UE_DEFINE_GAMEPLAY_TAG(Launch, "Event.Character.Launch");
		}

		namespace Boss
		{
			ASHENDUEL_API UE_DEFINE_GAMEPLAY_TAG(DeathCompleted, "Event.Boss.DeathCompleted");
			ASHENDUEL_API UE_DEFINE_GAMEPLAY_TAG(KnockDown, "Event.Boss.KnockDown");
			ASHENDUEL_API UE_DEFINE_GAMEPLAY_TAG(Phase2Changed, "Event.Boss.Phase2Changed");
		}
	}
	
	namespace Data
	{
		ASHENDUEL_API UE_DEFINE_GAMEPLAY_TAG(GroggyDamage, "Data.GroggyDamage");
		ASHENDUEL_API UE_DEFINE_GAMEPLAY_TAG(AttackDamage, "Data.AttackDamage");
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
			ASHENDUEL_API UE_DEFINE_GAMEPLAY_TAG(BasicAttack1, "Ability.Boss.BasicAttack1");
			ASHENDUEL_API UE_DEFINE_GAMEPLAY_TAG(BasicAttack2, "Ability.Boss.BasicAttack2");
			ASHENDUEL_API UE_DEFINE_GAMEPLAY_TAG(BasicAttack3, "Ability.Boss.BasicAttack3");
			ASHENDUEL_API UE_DEFINE_GAMEPLAY_TAG(BasicAttack4, "Ability.Boss.BasicAttack4");
			
			ASHENDUEL_API UE_DEFINE_GAMEPLAY_TAG(SpecialAttack1, "Ability.Boss.SpecialAttack1");
			ASHENDUEL_API UE_DEFINE_GAMEPLAY_TAG(SpecialAttack2, "Ability.Boss.SpecialAttack2");
			ASHENDUEL_API UE_DEFINE_GAMEPLAY_TAG(SpecialAttack3, "Ability.Boss.SpecialAttack3");
		}
	}
	
	namespace GameplayEvent
	{
		ASHENDUEL_API UE_DEFINE_GAMEPLAY_TAG(HitReact, "GameplayEvent.HitReact");
	}

	namespace Attack
	{
		ASHENDUEL_API UE_DEFINE_GAMEPLAY_TAG(AttackCombo1, "Attack.AttackCombo1");
		ASHENDUEL_API UE_DEFINE_GAMEPLAY_TAG(AttackCombo2, "Attack.AttackCombo2");
		ASHENDUEL_API UE_DEFINE_GAMEPLAY_TAG(AttackCombo3, "Attack.AttackCombo3");
		ASHENDUEL_API UE_DEFINE_GAMEPLAY_TAG(AttackCombo4, "Attack.AttackCombo4");
		ASHENDUEL_API UE_DEFINE_GAMEPLAY_TAG(HeavyAttack, "Attack.HeavyAttack");
		ASHENDUEL_API UE_DEFINE_GAMEPLAY_TAG(FatalAttack, "Attack.FatalAttack");
		
		ASHENDUEL_API UE_DEFINE_GAMEPLAY_TAG(AttackCombo3_Hit1, "Attack.AttackCombo3_Hit1");
		ASHENDUEL_API UE_DEFINE_GAMEPLAY_TAG(AttackCombo3_Hit2, "Attack.AttackCombo3_Hit2");
		ASHENDUEL_API UE_DEFINE_GAMEPLAY_TAG(AttackCombo4_Hit1, "Attack.AttackCombo4_Hit1");
		ASHENDUEL_API UE_DEFINE_GAMEPLAY_TAG(AttackCombo4_Hit2, "Attack.AttackCombo4_Hit2");
		
		ASHENDUEL_API UE_DEFINE_GAMEPLAY_TAG(HeavyAttack_Hit1, "Attack.HeavyAttack_Hit1");
		ASHENDUEL_API UE_DEFINE_GAMEPLAY_TAG(HeavyAttack_Hit2, "Attack.HeavyAttack_Hit2");
		ASHENDUEL_API UE_DEFINE_GAMEPLAY_TAG(HeavyAttack_Hit3, "Attack.HeavyAttack_Hit3");
		ASHENDUEL_API UE_DEFINE_GAMEPLAY_TAG(HeavyAttack_Hit4, "Attack.HeavyAttack_Hit4");
		ASHENDUEL_API UE_DEFINE_GAMEPLAY_TAG(HeavyAttack_Hit5, "Attack.HeavyAttack_Hit5");
		ASHENDUEL_API UE_DEFINE_GAMEPLAY_TAG(HeavyAttack_Hit6, "Attack.HeavyAttack_Hit6");
		
		
		ASHENDUEL_API UE_DEFINE_GAMEPLAY_TAG(FatalAttack_Hit1, "Attack.FatalAttack_Hit1");
		ASHENDUEL_API UE_DEFINE_GAMEPLAY_TAG(FatalAttack_Hit2, "Attack.FatalAttack_Hit2");
		ASHENDUEL_API UE_DEFINE_GAMEPLAY_TAG(FatalAttack_Hit3, "Attack.FatalAttack_Hit3");
		ASHENDUEL_API UE_DEFINE_GAMEPLAY_TAG(FatalAttack_Hit4, "Attack.FatalAttack_Hit4");
		ASHENDUEL_API UE_DEFINE_GAMEPLAY_TAG(FatalAttack_Hit5, "Attack.FatalAttack_Hit5");
		
		ASHENDUEL_API UE_DEFINE_GAMEPLAY_TAG(GroggyAttack, "Attack.GroggyAttack");

		namespace Boss
		{
			ASHENDUEL_API UE_DEFINE_GAMEPLAY_TAG(BasicAttack1, "Attack.Boss.BasicAttack1");
			ASHENDUEL_API UE_DEFINE_GAMEPLAY_TAG(BasicAttack2, "Attack.Boss.BasicAttack2");
			ASHENDUEL_API UE_DEFINE_GAMEPLAY_TAG(BasicAttack3, "Attack.Boss.BasicAttack3");
			
			ASHENDUEL_API UE_DEFINE_GAMEPLAY_TAG(BasicAttack4_Hit1, "Attack.Boss.BasicAttack4_Hit1");
			ASHENDUEL_API UE_DEFINE_GAMEPLAY_TAG(BasicAttack4_Hit2, "Attack.Boss.BasicAttack4_Hit2");
			ASHENDUEL_API UE_DEFINE_GAMEPLAY_TAG(BasicAttack4_Hit3, "Attack.Boss.BasicAttack4_Hit3");
			ASHENDUEL_API UE_DEFINE_GAMEPLAY_TAG(BasicAttack4_Hit4, "Attack.Boss.BasicAttack4_Hit4");

			ASHENDUEL_API UE_DEFINE_GAMEPLAY_TAG(SpecialAttack1_Hit1, "Attack.Boss.SpecialAttack1_Hit1");
			ASHENDUEL_API UE_DEFINE_GAMEPLAY_TAG(SpecialAttack1_Hit2, "Attack.Boss.SpecialAttack1_Hit2");
			ASHENDUEL_API UE_DEFINE_GAMEPLAY_TAG(SpecialAttack1_Hit3, "Attack.Boss.SpecialAttack1_Hit3");
			ASHENDUEL_API UE_DEFINE_GAMEPLAY_TAG(SpecialAttack1_Hit4, "Attack.Boss.SpecialAttack1_Hit4");
			ASHENDUEL_API UE_DEFINE_GAMEPLAY_TAG(SpecialAttack1_Hit5, "Attack.Boss.SpecialAttack1_Hit5");
			ASHENDUEL_API UE_DEFINE_GAMEPLAY_TAG(SpecialAttack1_Hit6, "Attack.Boss.SpecialAttack1_Hit6");
			
			ASHENDUEL_API UE_DEFINE_GAMEPLAY_TAG(SpecialAttack2_Hit1, "Attack.Boss.SpecialAttack2_Hit1");
			ASHENDUEL_API UE_DEFINE_GAMEPLAY_TAG(SpecialAttack2_Hit2, "Attack.Boss.SpecialAttack2_Hit2");
			ASHENDUEL_API UE_DEFINE_GAMEPLAY_TAG(SpecialAttack2_Hit3, "Attack.Boss.SpecialAttack2_Hit3");
			ASHENDUEL_API UE_DEFINE_GAMEPLAY_TAG(SpecialAttack2_Hit4, "Attack.Boss.SpecialAttack2_Hit4");
			ASHENDUEL_API UE_DEFINE_GAMEPLAY_TAG(SpecialAttack2_Hit5, "Attack.Boss.SpecialAttack2_Hit5");
			
			ASHENDUEL_API UE_DEFINE_GAMEPLAY_TAG(SpecialAttack3_Hit1, "Attack.Boss.SpecialAttack3_Hit1");
			ASHENDUEL_API UE_DEFINE_GAMEPLAY_TAG(SpecialAttack3_Hit2, "Attack.Boss.SpecialAttack3_Hit2");
			ASHENDUEL_API UE_DEFINE_GAMEPLAY_TAG(SpecialAttack3_Hit3, "Attack.Boss.SpecialAttack3_Hit3");
			ASHENDUEL_API UE_DEFINE_GAMEPLAY_TAG(SpecialAttack3_Hit4, "Attack.Boss.SpecialAttack3_Hit4");
		}
	}
}
