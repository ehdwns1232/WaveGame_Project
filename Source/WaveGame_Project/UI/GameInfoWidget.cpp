#include "UI/GameInfoWidget.h"
#include "Game/WaveGameInstance.h"
#include "Game/WaveGameState.h"
#include "Components/TextBlock.h"

void UGameInfoWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if (AWaveGameState* WaveGameState = Cast<AWaveGameState>(GetWorld()->GetGameState()))
	{
		WaveGameState->OnWaveChanged.AddDynamic(this, &UGameInfoWidget::UpdateWave);
		UpdateWave(WaveGameState->GetCurrentWaveIndex() + 1);
		WaveGameState->OnCoinChanged.AddDynamic(this, &UGameInfoWidget::UpdateCoin);
		UpdateCoin(WaveGameState->GetCollectedCoinCount(), WaveGameState->GetSpawnedCoinCount());
		WaveGameState->OnExplosionChanged.AddDynamic(this, &UGameInfoWidget::ShowExplosionEvent);
		if (ExplosionText) ExplosionText->SetVisibility(ESlateVisibility::Collapsed);
	}
	if (UWaveGameInstance* WaveGameInstance = Cast<UWaveGameInstance>(GetGameInstance()))
	{
		WaveGameInstance->OnLevelChanged.AddDynamic(this, &UGameInfoWidget::UpdateLevel);
		UpdateLevel(WaveGameInstance->GetCurrentLevelIndex() + 1);
	}

}

void UGameInfoWidget::UpdateLevel(int32 CurLevel)
{
	if (!LevelText) return;

	LevelText->SetText(FText::FromString(FString::Printf(TEXT("Level : %d"), CurLevel)));
}

void UGameInfoWidget::UpdateWave(int32 CurWave)
{
	if (!WaveText) return;

	WaveText->SetText(FText::FromString(FString::Printf(TEXT("Wave : %d"), CurWave)));
}

void UGameInfoWidget::UpdateCoin(int32 CollectedCoinCount, int32 TotalCoinCount)
{
	if (!CoinText) return;

	CoinText->SetText(FText::FromString(FString::Printf(TEXT("Remaining Coin : %d / %d"), TotalCoinCount - CollectedCoinCount, TotalCoinCount)));
}

void UGameInfoWidget::ShowExplosionEvent()
{
	ExplosionText->SetVisibility(ESlateVisibility::Visible);
	ExplosionText->SetText(FText::FromString(FString::Printf(TEXT("Explosion triggered"))));
	PlayAnimation(ExplosionAnim, 0.0f, 0);
	FTimerHandle CollapsedTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(CollapsedTimerHandle, this, &UGameInfoWidget::HideExplosionEvent, 1.0f, false);
}

void UGameInfoWidget::HideExplosionEvent()
{
	ExplosionText->SetVisibility(ESlateVisibility::Collapsed);
	StopAnimation(ExplosionAnim);
}



