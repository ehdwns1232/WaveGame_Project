#include "Item/SlowingItem.h"
#include "Player/PlayerCharacter.h"

ASlowingItem::ASlowingItem()
{
	ItemName = "Slow";
}

void ASlowingItem::ActivateItem(AActor* Activator)
{
	Super::ActivateItem(Activator);

	if (Activator && Activator->ActorHasTag("Player"))
	{
		if (APlayerCharacter* Player = Cast<APlayerCharacter>(Activator))
		{
			Player->AddSlowStack(SlowTime);
		}
		DestroyItem();
	}
}

