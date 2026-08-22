#include "WaveGameInstance.h"

UWaveGameInstance::UWaveGameInstance()
{
}

void UWaveGameInstance::AddToScore(int32 Amount)
{
	TotalScore += Amount;
	UE_LOG(LogTemp, Warning, TEXT("TotalScore : %f"), TotalScore);
}

float UWaveGameInstance::GetTotalScore() const
{
	return TotalScore;
}
int32 UWaveGameInstance::GetCurrentLevelIndex() const
{
	return CurrentLevelIndex;
}
void UWaveGameInstance::SetCurrentLevelIndex(int32 NewCurrentLevelIndex)
{
	CurrentLevelIndex = NewCurrentLevelIndex;
}

UDataTable* UWaveGameInstance::GetCurrentTable() const
{
	if (!LevelTables.IsValidIndex(CurrentLevelIndex)) return nullptr;

	return LevelTables[CurrentLevelIndex];
}
