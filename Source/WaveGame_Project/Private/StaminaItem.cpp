#include "StaminaItem.h"
#include "PlayerCharacter.h"

AStaminaItem::AStaminaItem()
{
	ItemName = "Stamina";
}

void AStaminaItem::ActivateItem(AActor* Activator)
{
	Super::ActivateItem(Activator);

	if (Activator && Activator->ActorHasTag(TEXT("Player")))
	{
		if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(Activator))
		{
			PlayerCharacter->AddStamina(StaminaAmount);
		}
	}
	DestroyItem();
}
