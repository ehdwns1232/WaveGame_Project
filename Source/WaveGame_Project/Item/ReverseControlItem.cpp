#include "Item/ReverseControlItem.h"
#include "Player/PlayerCharacter.h"

AReverseControlItem::AReverseControlItem()
{
	ItemName = "Reverse";
}

void AReverseControlItem::ActivateItem(AActor* Activator)
{
	Super::ActivateItem(Activator);

	if (Activator && Activator->ActorHasTag("Player"))
	{
		if (APlayerCharacter* Player = Cast<APlayerCharacter>(Activator))
		{
			Player->AddReverseStack(ReverseTime);
		}
		DestroyItem();
	}
}
