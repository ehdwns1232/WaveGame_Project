#include "Game/WaveGameState.h"
#include "Kismet/GameplayStatics.h"
#include "SpawnVolume/SpawnVolume.h"
#include "Item/CoinItem.h"
#include "Game/WaveGameInstance.h"
#include "Obstacle/ObstacleBase.h"
#include "Obstacle/FallingObstacle.h"
#include "Obstacle/ExpandingObstacle.h"
#include "Obstacle/ExplosionObstacle.h"
#include "Player/WavePlayerController.h"
#include "Player/PlayerCharacter.h"

AWaveGameState::AWaveGameState()
{
	
}

void AWaveGameState::BeginPlay()
{
	Super::BeginPlay();

	WaveGameInstance = Cast<UWaveGameInstance>(GetGameInstance());

	FString CurrentMapName = GetWorld()->GetMapName();
	if (!CurrentMapName.Contains(TEXT("MenuLevel"), ESearchCase::IgnoreCase))
	{
		StartLevel();
	}
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
	if (WaveGameInstance)
	{
		Score += Amount;
		OnScoreChanged.Broadcast(Score);
		WaveGameInstance->AddToScore(Amount);
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

	for (AActor* Actor : ExplosionObstacles)
	{
		if (Actor) Actor->Destroy();
	}
}

void AWaveGameState::OnNextLevel()
{
	CurrentLevelIndex++;

	if (CurrentLevelIndex >= MaxLevel)
	{
		OnGameOver(true);
		return;
	}

	WaveGameInstance->SetCurrentLevelIndex(CurrentLevelIndex);
	
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
	// Wave 시작 | 점수, 코인 갯수, 플레이어 디버프, 위치 초기화
	UE_LOG(LogTemp, Warning, TEXT("Wave %d Start"), CurrentWaveIndex + 1);
	Score = 0;
	SpawnedCoinCount = 0;
	CollectedCoinCount = 0;
	OnScoreChanged.Broadcast(Score);
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn()))
	{
		PlayerCharacter->RemoveAllDebuff();
		PlayerCharacter->SetActorLocation(FVector::ZeroVector);
	}

	// LevelIndex, GoalScore, WaveDuration 초기화 해주고 데이터 테이블기반 스폰정보 가져오기
	TArray<AActor*> FoundVolumes;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnVolume::StaticClass(), FoundVolumes);
	if (FoundVolumes.IsEmpty()) return;

	ASpawnVolume* SpawnVolume = Cast<ASpawnVolume>(FoundVolumes[0]);
	if (!SpawnVolume) return;

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
	WaveDuration = CurrentWave->WaveDuration;

	// 스폰
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
			else if (ActualClass->IsChildOf(AExplosionObstacle::StaticClass()))
			{
				TempArray = SpawnVolume->SpawnObstacleBottom(ActualClass, SpawnItemInfo.SpawnCount);
				for (AActor* Actor : TempArray)
				{
					ExplosionObstacles.Add(Cast<AExplosionObstacle>(Actor));
				}
			}
		}
	}

	OnCoinChanged.Broadcast(CollectedCoinCount, SpawnedCoinCount);
	SetTimerHandle();
}

void AWaveGameState::OnNextWave()
{
	ClearTimerHandle();
	CurrentWaveIndex++;
	OnWaveChanged.Broadcast(CurrentWaveIndex + 1);
	DestroyAllSpawned();
	FallingObstacles.Empty();
	ExpandingObstacles.Empty();
	ExplosionObstacles.Empty();

	if (CurrentWaveIndex >= MaxWave)
	{
		OnNextLevel();
		return;
	}

	StartWave();
}

void AWaveGameState::OnGameOver(bool bIsClear)
{
	if (AWavePlayerController* WavePlayerController = Cast<AWavePlayerController>(GetWorld()->GetFirstPlayerController()))
	{
		WavePlayerController->SetPause(true);
		WavePlayerController->ShowMainMenu(true, bIsClear);
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
	else if (ObstacleType == EObstacleType::Explosion)
	{
		for (AExplosionObstacle* Object : ExplosionObstacles)
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

void AWaveGameState::NotifyExplosionChanged()
{
	OnExplosionChanged.Broadcast();
}

void AWaveGameState::SetTimerHandle()
{
	FTimerDelegate TimeOverDelegate;
	TimeOverDelegate.BindUObject(this, &AWaveGameState::OnGameOver, false);
	GetWorldTimerManager().SetTimer(WaveTimerHandle, TimeOverDelegate, WaveDuration, false);
	FTimerDelegate FallDelegate;
	FallDelegate.BindUObject(this, &AWaveGameState::OnOperateObstacle, EObstacleType::Fall);
	GetWorldTimerManager().SetTimer(FallTimerHandle, FallDelegate, FallDelay, true);
	FTimerDelegate ExpandDelegate;
	ExpandDelegate.BindUObject(this, &AWaveGameState::OnOperateObstacle, EObstacleType::Expand);
	GetWorldTimerManager().SetTimer(ExpandTimerHandle, ExpandDelegate, ExpandDelay, true);
	if (CurrentWaveIndex == 2)
	{
		FTimerDelegate ExplosionDelegate;
		ExplosionDelegate.BindUObject(this, &AWaveGameState::OnOperateObstacle, EObstacleType::Explosion);
		GetWorldTimerManager().SetTimer(ExplosionTimerHandle, ExplosionDelegate, ExplosionDelay, true);
	}
}

void AWaveGameState::ClearTimerHandle()
{
	GetWorldTimerManager().ClearTimer(WaveTimerHandle);
	GetWorldTimerManager().ClearTimer(FallTimerHandle);
	GetWorldTimerManager().ClearTimer(ExpandTimerHandle);
	GetWorldTimerManager().ClearTimer(ExplosionTimerHandle);
}


