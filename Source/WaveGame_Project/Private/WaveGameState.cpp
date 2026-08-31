#include "WaveGameState.h"
#include "Kismet/GameplayStatics.h"
#include "SpawnVolume.h"
#include "CoinItem.h"
#include "WaveGameInstance.h"
#include "ObstacleBase.h"
#include "FallingObstacle.h"
#include "ExpandingObstacle.h"
#include "WavePlayerController.h"
#include "PlayerCharacter.h"

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
	ClearTimerHandle();

	Super::EndPlay(EndPlayReason);
}

int32 AWaveGameState::GetScore() const
{
	return Score;
}

int32 AWaveGameState::GetGoalScore() const
{
	return WaveGoalScore;
}

void AWaveGameState::AddScore(int32 Amount)
{
	UWaveGameInstance* WaveGameInstance = Cast<UWaveGameInstance>(GetGameInstance());
	{
		if (WaveGameInstance)
		{
			Score += Amount;
			OnScoreChanged.Broadcast(Score);
			WaveGameInstance->AddToScore(Amount);
		}
	}
}

int32 AWaveGameState::GetCurrentWaveIndex() const
{
	return CurrentWaveIndex;
}

int32 AWaveGameState::GetCollectedCoinCount() const
{
	return CollectedCoinCount;
}

int32 AWaveGameState::GetSpawnedCoinCount() const
{
	return SpawnedCoinCount;
}

void AWaveGameState::StartLevel()
{
	if (AWavePlayerController* WavePlayerController = Cast<AWavePlayerController>(GetWorld()->GetFirstPlayerController()))
	{
		WavePlayerController->ShowGameHUD();
	}
	CurrentWaveIndex = 0;
	OnWaveChanged.Broadcast(CurrentWaveIndex + 1);
	StartWave();
}

void AWaveGameState::OnCoinCollected()
{
	CollectedCoinCount++;
	OnCoinChanged.Broadcast(CollectedCoinCount, SpawnedCoinCount);
	if (Score >= WaveGoalScore || CollectedCoinCount >= SpawnedCoinCount)
	{
		OnNextWave();
	}
}

void AWaveGameState::DestroyAllSpawned()
{
	for (AActor* Actor : AllItems)
	{
		if (Actor) Actor->Destroy();
	}

	for (AActor* Actor : FallingObstacles)
	{
		if (Actor) Actor->Destroy();
	}

	for (AActor* Actor : ExpandingObstacles)
	{
		if (Actor) Actor->Destroy();
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
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, FString::Printf(TEXT("Wave %d Start !"), CurrentWaveIndex + 1));
	Score = 0;
	SpawnedCoinCount = 0;
	CollectedCoinCount = 0;
	OnScoreChanged.Broadcast(Score);
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn()))
	{
		PlayerCharacter->RemoveAllDebuff();
		PlayerCharacter->SetActorLocation(FVector::ZeroVector);
	}

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
	WaveGoalScore = CurrentWave->GoalScore;
	OnGoalScoreChanged.Broadcast(WaveGoalScore);

	for (const auto& SpawnItemInfo : CurrentWave->SpawnItemInfo)
	{
		if (UClass* ActualClass = SpawnItemInfo.SpawnClass)
		{
			TArray<AActor*> TempArray;
			
			if (ActualClass->IsChildOf(ABaseItem::StaticClass()))
			{
				TempArray = SpawnVolume->SpawnItem(ActualClass, SpawnItemInfo.SpawnCount);
				for (AActor* Actor : TempArray)
				{
					AllItems.Add(Cast<ABaseItem>(Actor));
				}

				if (ActualClass->IsChildOf(ACoinItem::StaticClass()))
				{
					SpawnedCoinCount += SpawnItemInfo.SpawnCount;
				}
			}
			else if (ActualClass->IsChildOf(AFallingObstacle::StaticClass()))
			{
				TempArray = SpawnVolume->SpawnObstacleTop(ActualClass, SpawnItemInfo.SpawnCount);
				for (AActor* Actor : TempArray)
				{
					FallingObstacles.Add(Cast<AFallingObstacle>(Actor));
				}
			}
			else if (ActualClass->IsChildOf(AExpandingObstacle::StaticClass()))
			{
				TempArray = SpawnVolume->SpawnObstacleSide(ActualClass, SpawnItemInfo.SpawnCount);
				for (AActor* Actor : TempArray)
				{
					ExpandingObstacles.Add(Cast<AExpandingObstacle>(Actor));
				}
			}
		}
	}

	OnCoinChanged.Broadcast(CollectedCoinCount, SpawnedCoinCount);
	WaveDuration = CurrentWave->WaveDuration;
	SetTimerHandle();
}

void AWaveGameState::OnNextWave()
{
	UWaveGameInstance* WaveGameInstance = Cast<UWaveGameInstance>(GetGameInstance());
	if (!WaveGameInstance) return;

	ClearTimerHandle();
	CurrentWaveIndex++;
	OnWaveChanged.Broadcast(CurrentWaveIndex + 1);
	FallingObstacles.Empty();
	ExpandingObstacles.Empty();
	DestroyAllSpawned();

	if (CurrentWaveIndex >= MaxWave)
	{
		OnNextLevel();
		return;
	}

	StartWave();
}

void AWaveGameState::OnGameOver()
{
	if (AWavePlayerController* WavePlayerController = Cast<AWavePlayerController>(GetWorld()->GetFirstPlayerController()))
	{
		WavePlayerController->SetPause(true);
		WavePlayerController->ShowMainMenu(true);
	}
}

void AWaveGameState::OnOperateObstacle(EObstacleType ObstacleType)
{
	TArray<AObstacleBase*> Obstacles;
	if (ObstacleType == EObstacleType::Fall)
	{
		for (AFallingObstacle* Object : FallingObstacles)
		{
			if (Object && !Object->GetCanActivate()) Obstacles.Add(Object);
		}

	}
	else if (ObstacleType == EObstacleType::Expand)
	{
		for (AExpandingObstacle* Object : ExpandingObstacles)
		{
			if (Object && !Object->GetCanActivate()) Obstacles.Add(Object);
		}
	}

	int32 MaxRandCount = (CurrentLevelIndex + 1) * 2 + (CurrentWaveIndex + 1) * 2;
	int32 RandCount = FMath::RandRange(2, MaxRandCount);
	int32 OperateCount = FMath::Min(RandCount, Obstacles.Num());

	for (int32 i = 0; i < OperateCount; ++i)
	{
		int32 RandIndex = FMath::RandRange(0, Obstacles.Num() - 1);

		Obstacles[RandIndex]->ActivateObstacle();

		Obstacles.RemoveAt(RandIndex);
	}
}

void AWaveGameState::SetTimerHandle()
{
	GetWorldTimerManager().SetTimer(WaveTimerHandle, this, &AWaveGameState::OnGameOver, WaveDuration, false);
	FTimerDelegate FallDelegate;
	FallDelegate.BindUObject(this, &AWaveGameState::OnOperateObstacle, EObstacleType::Fall);
	GetWorldTimerManager().SetTimer(FallTimerHandle, FallDelegate, FallDelay, true);
	FTimerDelegate ExpandDelegate;
	ExpandDelegate.BindUObject(this, &AWaveGameState::OnOperateObstacle, EObstacleType::Expand);
	GetWorldTimerManager().SetTimer(ExpandTimerHandle, ExpandDelegate, ExpandDelay, true);
}

void AWaveGameState::ClearTimerHandle()
{
	GetWorldTimerManager().ClearTimer(WaveTimerHandle);
	GetWorldTimerManager().ClearTimer(FallTimerHandle);
	GetWorldTimerManager().ClearTimer(ExpandTimerHandle);
}


