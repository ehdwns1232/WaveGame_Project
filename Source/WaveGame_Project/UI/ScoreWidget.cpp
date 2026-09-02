#include "UI/ScoreWidget.h"
#include "Game/WaveGameState.h"
#include "Game/WaveGameInstance.h"
#include "Components/TextBlock.h"

void UScoreWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if (AWaveGameState* WaveGameState = Cast<AWaveGameState>(GetWorld()->GetGameState()))
	{
		WaveGameState->OnScoreChanged.AddDynamic(this, &UScoreWidget::UpdateScore);
		UpdateScore(WaveGameState->GetScore());
		WaveGameState->OnGoalScoreChanged.AddDynamic(this, &UScoreWidget::UpdateGoalScore);
		UpdateGoalScore(WaveGameState->GetGoalScore());
	}
	if (UWaveGameInstance* WaveGameInstance = Cast<UWaveGameInstance>(GetGameInstance()))
	{
		WaveGameInstance->OnTotalScoreChanged.AddDynamic(this, &UScoreWidget::UpdateTotalScore);
		UpdateTotalScore(WaveGameInstance->GetTotalScore());
	}
}

void UScoreWidget::UpdateScore(int32 CurScore)
{
	if (!ScoreText) return;

	ScoreText->SetText(FText::FromString(FString::Printf(TEXT("Score : %d"), CurScore)));
}

void UScoreWidget::UpdateGoalScore(int32 GoalScore)
{
	if (!GoalScoreText) return;

	GoalScoreText->SetText(FText::FromString(FString::Printf(TEXT("Goal Score : %d"), GoalScore)));
}

void UScoreWidget::UpdateTotalScore(int32 TotalScore)
{
	if (!TotalScoreText) return;

	TotalScoreText->SetText(FText::FromString(FString::Printf(TEXT("Total Score : %d"), TotalScore)));
}
