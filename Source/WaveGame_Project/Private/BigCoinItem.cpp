#include "BigCoinItem.h"

ABigCoinItem::ABigCoinItem()
{
	ItemName = "BigCoin";
	PointValue = 20;
}

void ABigCoinItem::ActivateItem(AActor* Activator)
{
	Super::ActivateItem(Activator);
}
