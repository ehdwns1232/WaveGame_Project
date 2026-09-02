#include "Item/CoinItem.h"
#include "Engine/World.h"
#include "Game/WaveGameState.h"

ACoinItem::ACoinItem()
{
	ItemName = "DefaultCoin";
}

void ACoinItem::ActivateItem(AActor* Activator)
{
	Super::ActivateItem(Activator);
	if (Activator && Activator->ActorHasTag("Player"))
	{
		if (UWorld* World = GetWorld())
		{
			if (AWaveGameState* GameState = World->GetGameState<AWaveGameState>())
			{
				GameState->AddScore(PointValue);
				GameState->OnCoinCollected();
			}
		}
		DestroyItem();
	}
}
