#include "HealingItem.h"
#include "PlayerCharacter.h"

AHealingItem::AHealingItem()
{
	ItemType = "HealingItem";
	HealAmount = 20;
}

void AHealingItem::ActivateItem(AActor* Activator)
{
	if (Activator && Activator->ActorHasTag("Player"))
	{
		if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(Activator))
		{
			PlayerCharacter->AddHealth(HealAmount);
		}
		DestroyItem();
	}
}
