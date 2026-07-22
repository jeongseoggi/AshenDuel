// Fill out your copyright notice in the Description page of Project Settings.


#include "ADPotionWidget.h"

#include "AbilitySystemComponent.h"
#include "AshenDuel/GAS/AttributeSet/ADPlayerAttributeSet.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UADPotionWidget::InitAbilitySystem(UAbilitySystemComponent* InASC)
{
	ASC = InASC;

	if (!ASC)
	{
		return;
	}
	
	ASC->GetGameplayAttributeValueChangeDelegate(
		UADPlayerAttributeSet::GetPotionCountAttribute()).AddUObject(
			this, &UADPotionWidget::OnPotionCountValueChanged);
}

void UADPotionWidget::OnPotionCountValueChanged(const FOnAttributeChangeData& Data)
{
	int32 PotionCount = ASC->GetNumericAttribute(UADPlayerAttributeSet::GetPotionCountAttribute());
	PotionCountText->SetText(FText::FromString(FString::Printf(TEXT("%d"), PotionCount)));
	
	if (PotionCount == 0)
		EmptyPotion();
}

void UADPotionWidget::EmptyPotion()
{
	if (PotionImage)
	{
		FLinearColor CurrentColor = PotionImage->GetColorAndOpacity();
		CurrentColor.A = 0.5f;
		PotionImage->SetBrushTintColor(FSlateColor(CurrentColor));
	}
}
