#include "UI/ItemWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void UItemWidget::SetItemInfo(FName ItemName, UTexture2D* ItemIcon)
{
	if (!NameText || !IconImage) return;

	NameText->SetText(FText::FromName(ItemName));
	IconImage->SetBrushFromTexture(ItemIcon);
}
