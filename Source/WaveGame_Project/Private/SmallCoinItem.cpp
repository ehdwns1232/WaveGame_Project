#include "SmallCoinItem.h"

ASmallCoinItem::ASmallCoinItem()
{
	ItemName = "SmallCoin";
	PointValue = 10;
}

void ASmallCoinItem::ActivateItem(AActor* Activator)
{
	Super::ActivateItem(Activator);


}
