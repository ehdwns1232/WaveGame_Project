#include "HealingItem.h"
#include "PlayerCharacter.h"

AHealingItem::AHealingItem()
{
	ItemName = "Healing";
}

void AHealingItem::ActivateItem(AActor* Activator)
{
	Super::ActivateItem(Activator);
	if (Activator && Activator->ActorHasTag("Player"))
	{
		if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(Activator))
		{
			PlayerCharacter->AddHealth(HealAmount);
		}
		DestroyItem();
	}
}
