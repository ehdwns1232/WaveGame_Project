#include "SlowingItem.h"
#include "PlayerCharacter.h"

ASlowingItem::ASlowingItem()
{
	ItemName = "Slowing";
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

