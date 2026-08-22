#include "CoinItem.h"
#include "Engine/World.h"
#include "WaveGameState.h"

ACoinItem::ACoinItem()
{
	ItemType = "DefaultCoin";
	PointValue = 0;
}

void ACoinItem::ActivateItem(AActor* Activator)
{
	if (Activator && Activator->ActorHasTag("Player"))
	{
		if (UWorld* World = GetWorld())
		{
			if (AWaveGameState* GameState = World->GetGameState<AWaveGameState>())
			{
				GameState->AddScore(PointValue);
				GameState->OnCoinCollected();
				UE_LOG(LogTemp, Warning, TEXT("Player Gained Point : %d"), PointValue);
			}
		}
		DestroyItem();
	}
}
