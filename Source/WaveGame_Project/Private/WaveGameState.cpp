#include "WaveGameState.h"

AWaveGameState::AWaveGameState()
{
	Score = 0;
}

void AWaveGameState::BeginPlay()
{
	Super::BeginPlay();

	StartLevel();
}

int32 AWaveGameState::GetScore() const
{
	return Score;
}

void AWaveGameState::AddScore(int32 Amount)
{
	Score += Amount;
}

void AWaveGameState::StartLevel()
{

}
