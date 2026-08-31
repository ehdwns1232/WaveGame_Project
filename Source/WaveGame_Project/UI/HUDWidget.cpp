#include "UI/HUDWidget.h"
#include "WaveGameInstance.h"
#include "WaveGameState.h"
#include "Components/TextBlock.h"
#include "PlayerStatsWidget.h"
#include "ScoreWidget.h"
#include "DebuffWidget.h"
#include "TimeWidget.h"

void UHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if (AWaveGameState* WaveGameState = Cast<AWaveGameState>(GetWorld()->GetGameState()))
	{
		WaveGameState->OnWaveChanged.AddDynamic(this, &UHUDWidget::UpdateWave);
		UpdateWave(WaveGameState->GetCurrentWaveIndex() + 1);
		WaveGameState->OnCoinChanged.AddDynamic(this, &UHUDWidget::UpdateCoin);
		UpdateCoin(WaveGameState->GetCollectedCoinCount(), WaveGameState->GetSpawnedCoinCount());
	}
	if (UWaveGameInstance* WaveGameInstance = Cast<UWaveGameInstance>(GetGameInstance()))
	{
		WaveGameInstance->OnLevelChanged.AddDynamic(this, &UHUDWidget::UpdateLevel);
		UpdateLevel(WaveGameInstance->GetCurrentLevelIndex() + 1);
	}
}

void UHUDWidget::UpdateLevel(int32 CurLevel)
{
	if (!LevelText) return;

	LevelText->SetText(FText::FromString(FString::Printf(TEXT("Level : %d"), CurLevel)));
}

void UHUDWidget::UpdateWave(int32 CurWave)
{
	if (!WaveText) return;

	WaveText->SetText(FText::FromString(FString::Printf(TEXT("Wave : %d"), CurWave)));
}

void UHUDWidget::UpdateCoin(int32 CollectedCoinCount, int32 TotalCoinCount)
{
	if (!CoinText) return;

	CoinText->SetText(FText::FromString(FString::Printf(TEXT("Remaining Coin : %d / %d"), TotalCoinCount - CollectedCoinCount, TotalCoinCount))); 
}


