#include "BigCoinItem.h"

ABigCoinItem::ABigCoinItem()
{
	ItemName = "BigCoin";
	PointValue = 30;
}

void ABigCoinItem::ActivateItem(AActor* Activator)
{
	Super::ActivateItem(Activator);
}
