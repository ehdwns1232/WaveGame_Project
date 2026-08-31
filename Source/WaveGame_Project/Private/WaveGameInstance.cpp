#include "WaveGameInstance.h"

UWaveGameInstance::UWaveGameInstance()
{
}

void UWaveGameInstance::AddToScore(int32 Amount)
{
	TotalScore += Amount;

	OnTotalScoreChanged.Broadcast(TotalScore);
}

int32 UWaveGameInstance::GetTotalScore() const
{
	return TotalScore;
}
void UWaveGameInstance::SetTotalScore(int32 Amount)
{
	TotalScore = Amount;

	OnTotalScoreChanged.Broadcast(TotalScore);
}
int32 UWaveGameInstance::GetCurrentLevelIndex() const
{
	return CurrentLevelIndex;
}
void UWaveGameInstance::SetCurrentLevelIndex(int32 NewCurrentLevelIndex)
{
	CurrentLevelIndex = NewCurrentLevelIndex;

	OnLevelChanged.Broadcast(CurrentLevelIndex + 1);
}

UDataTable* UWaveGameInstance::GetCurrentTable() const
{
	if (!LevelTables.IsValidIndex(CurrentLevelIndex)) return nullptr;

	return LevelTables[CurrentLevelIndex];
}
