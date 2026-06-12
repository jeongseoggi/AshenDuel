#include "ADAbilitySystemComponent.h"


void UADAbilitySystemComponent::AbilityInputTagPressed(const FGameplayTag& InputTag)
{
	if (InputTag.IsValid())
	{
		ABILITYLIST_SCOPE_LOCK();
		
		for (FGameplayAbilitySpec& Spec : ActivatableAbilities.Items)
		{
			if (Spec.Ability && Spec.DynamicAbilityTags.HasTagExact(InputTag))
			{
				AbilitySpecInputPressed(Spec);
				if (!Spec.IsActive())
				{
					TryActivateAbility(Spec.Handle);
				}
			}
		}
	}
}

void UADAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;

	ABILITYLIST_SCOPE_LOCK();

	for (FGameplayAbilitySpec& Spec : ActivatableAbilities.Items)
	{
		if (Spec.Ability && Spec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputReleased(Spec);
		}
	}
}
