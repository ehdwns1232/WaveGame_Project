#include "WaveGameState.h"
#include "Kismet/GameplayStatics.h"
#include "SpawnVolume.h"
#include "CoinItem.h"
#include "WaveGameInstance.h"
#include "FallingObstacle.h"
#include "ExpandingObstacle.h"

AWaveGameState::AWaveGameState()
{
	
}

void AWaveGameState::BeginPlay()
{
	Super::BeginPlay();

	StartLevel();
}

void AWaveGameState::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	GetWorldTimerManager().ClearTimer(WaveTimerHandle);
	GetWorldTimerManager().ClearTimer(FallTimerHandle);
	GetWorldTimerManager().ClearTimer(ExpandTimerHandle);

	Super::EndPlay(EndPlayReason);
}

int32 AWaveGameState::GetScore() const
{
	return Score;
}

void AWaveGameState::AddScore(int32 Amount)
{
	UWaveGameInstance* WaveGameInstance = Cast<UWaveGameInstance>(GetGameInstance());
	{
		if (WaveGameInstance)
		{
			Score += Amount;
			WaveGameInstance->AddToScore(Amount);
		}
	}
}

void AWaveGameState::StartLevel()
{
	CurrentWaveIndex = 0;
	SpawnedCoinCount = 0;
	CollectedCoinCount = 0;
	
	StartWave();
}

void AWaveGameState::OnCoinCollected()
{
	CollectedCoinCount++;
	if (CollectedCoinCount >= SpawnedCoinCount)
	{
		OnNextWave();
	}
}

void AWaveGameState::DestroyAllSpawned()
{
	TArray<AActor*> SpawnedItems;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABaseItem::StaticClass(), SpawnedItems);

	if (SpawnedItems.Num() > 0)
	{
		for (AActor* Actor : SpawnedItems)
		{
			if (Actor) Actor->Destroy();
		}
	}

	TArray<AActor*> SpawnedObstacles;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AObstacleBase::StaticClass(), SpawnedObstacles);
	if (SpawnedObstacles.Num() > 0)
	{
		for (AActor* Actor : SpawnedObstacles)
		{
			if (Actor) Actor->Destroy();
		}
	}
}

void AWaveGameState::OnNextLevel()
{
	CurrentLevelIndex++;

	if (CurrentLevelIndex >= MaxLevel)
	{
		OnGameOver();
		return;
	}

	UWaveGameInstance* WaveGameInstance = Cast<UWaveGameInstance>(GetGameInstance());
	{
		if (WaveGameInstance)
		{
			WaveGameInstance->SetCurrentLevelIndex(CurrentLevelIndex);
		}
	}

	EndLevel();
}

void AWaveGameState::EndLevel()
{
	if (LevelNames.IsValidIndex(CurrentLevelIndex))
	{
		UGameplayStatics::OpenLevel(GetWorld(), LevelNames[CurrentLevelIndex]);
	}
}

void AWaveGameState::StartWave()
{
	TArray<AActor*> FoundVolumes;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnVolume::StaticClass(), FoundVolumes);
	if (FoundVolumes.IsEmpty()) return;

	ASpawnVolume* SpawnVolume = Cast<ASpawnVolume>(FoundVolumes[0]);
	if (!SpawnVolume) return;

	UWaveGameInstance* WaveGameInstance = Cast<UWaveGameInstance>(GetGameInstance());
	if (!WaveGameInstance) return;

	CurrentLevelIndex = WaveGameInstance->GetCurrentLevelIndex();
	UDataTable* LevelTable = WaveGameInstance->GetCurrentTable();
	if (!LevelTable) return;

	TArray<FWaveDataRow*> WaveAllRows;
	LevelTable->GetAllRows<FWaveDataRow>(TEXT("WaveDataRow Context"), WaveAllRows);
	if (!WaveAllRows.IsValidIndex(CurrentWaveIndex)) return;

	FWaveDataRow* CurrentWave = WaveAllRows[CurrentWaveIndex];
	if (!CurrentWave) return;

	for (const auto& SpawnItemInfo : CurrentWave->SpawnItemInfo)
	{
		if (UClass* ActualClass = SpawnItemInfo.SpawnClass)
		{
			TArray<AActor*> TempArray;
			
			if (ActualClass->IsChildOf(ABaseItem::StaticClass()))
			{
				SpawnVolume->SpawnItem(ActualClass, SpawnItemInfo.SpawnCount);
				if (ActualClass->IsChildOf(ACoinItem::StaticClass()))
				{
					SpawnedCoinCount += SpawnItemInfo.SpawnCount;
				}
			}
			else if (ActualClass->IsChildOf(AFallingObstacle::StaticClass()))
			{
				UE_LOG(LogTemp, Warning, TEXT("State Top Start"));
				TempArray = SpawnVolume->SpawnObstacleTop(ActualClass, SpawnItemInfo.SpawnCount);
				for (AActor* Actor : TempArray)
				{
					FallingObstacles.Add(Cast<AFallingObstacle>(Actor));
				}
			}
			else if (ActualClass->IsChildOf(AExpandingObstacle::StaticClass()))
			{
				UE_LOG(LogTemp, Warning, TEXT("State Side Start"));
				TempArray = SpawnVolume->SpawnObstacleSide(ActualClass, SpawnItemInfo.SpawnCount);
				for (AActor* Actor : TempArray)
				{
					ExpandingObstacles.Add(Cast<AExpandingObstacle>(Actor));
				}
			}
			
		}
	}

	GetWorldTimerManager().SetTimer(WaveTimerHandle, this, &AWaveGameState::OnGameOver, CurrentWave->WaveTimeDuration, false);
	GetWorldTimerManager().SetTimer(FallTimerHandle, this, &AWaveGameState::OnFall, FallDelay, true);
	GetWorldTimerManager().SetTimer(ExpandTimerHandle, this, &AWaveGameState::OnExpand, ExpandDelay, true);
	UE_LOG(LogTemp, Warning, TEXT("Level : %d Start! Wave : %d Start! SpawnedCoin : %d coin"), CurrentLevelIndex + 1, CurrentWaveIndex, SpawnedCoinCount);
}

void AWaveGameState::OnNextWave()
{
	GetWorldTimerManager().ClearTimer(WaveTimerHandle);
	Score = 0;
	SpawnedCoinCount = 0;
	CurrentWaveIndex++;
	DestroyAllSpawned();
	FallingObstacles.Empty();
	ExpandingObstacles.Empty();

	if (CurrentWaveIndex >= MaxWave)
	{
		OnNextLevel();
		return;
	}

	StartWave();
}

void AWaveGameState::OnGameOver()
{
	
	// UI 열기
}

void AWaveGameState::OnFall()
{
	if (FallingObstacles.IsEmpty()) return;

	TArray<AFallingObstacle*> OnSkyObstacles;
	for (AFallingObstacle* Obstacle : FallingObstacles)
	{
		if (Obstacle && !Obstacle->GetIsGround())
		{
			OnSkyObstacles.Add(Obstacle);
		}
	}

	int32 RandCount = FMath::RandRange(1, 5);
	int32 FallCount = FMath::Min(RandCount, OnSkyObstacles.Num());

	for (int i = 0; i < FallCount; ++i)
	{
		int32 RandIndex = FMath::RandRange(0, OnSkyObstacles.Num() - 1);
		UE_LOG(LogTemp, Error, TEXT("Fall Current Index : %d"), i);
		OnSkyObstacles[RandIndex]->StartFall();

		OnSkyObstacles.RemoveAtSwap(RandIndex);
	}

}

void AWaveGameState::OnExpand()
{
	if (ExpandingObstacles.IsEmpty()) return;

	TArray<AExpandingObstacle*> AllObject;
	for (AExpandingObstacle* Obstacle : ExpandingObstacles)
	{
		if (Obstacle && !Obstacle->GetIsExpanding())
		{
			AllObject.Add(Obstacle);
		}
	}

	int32 MaxFallCount = (CurrentLevelIndex + 1) * 2 + (CurrentWaveIndex + 1) * 2;
	int32 RandCount = FMath::RandRange(2, MaxFallCount);
	int32 ExpandCount = FMath::Min(RandCount, AllObject.Num());

	for (int32 i = 0; i < RandCount; ++i)
	{
		int32 RandIndex = FMath::RandRange(0, AllObject.Num() - 1);
		UE_LOG(LogTemp, Error, TEXT("Expand Current Index : %d"), i);
		ExpandingObstacles[RandIndex]->StartExpand();

		AllObject.RemoveAt(RandIndex);
	}
}
