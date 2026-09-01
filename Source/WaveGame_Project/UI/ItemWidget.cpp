#include "UI/ItemWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void UItemWidget::SetItemInfo(FName ItemName, UTexture2D* ItemIcon, bool bIsAutoConsumable)
{
	if (!NameText || !IconImage || !PickupText) return;

	NameText->SetText(FText::FromName(ItemName));
	IconImage->SetBrushFromTexture(ItemIcon);
	PickupText->SetText(FText::FromString(FString::Printf(TEXT("Press F to pickup"))));
	SetPickupTextVisible(bIsAutoConsumable);
}

void UItemWidget::SetPickupTextVisible(bool bIsAutoConsumable)
{
	if (bIsAutoConsumable)
	{
		PickupText->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		PickupText->SetVisibility(ESlateVisibility::Visible);
	}
}

void UItemWidget::PlayPickupAnim()
{
	PlayAnimation(PickupAnim, 0.0f, 0);
}

void UItemWidget::StopPickupAnim()
{
	StopAnimation(PickupAnim);
}