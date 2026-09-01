#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "WaveGameState.generated.h"

class ABaseItem;
class AFallingObstacle;
class AExpandingObstacle;
class AExplosionObstacle;
class UWaveGameInstance;

UENUM(BlueprintType)
enum class EObstacleType : uint8
{
	Fall,
	Expand,
	Explosion
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWaveChanged, int32, CurWave);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnScoreChanged, int32, CurScore);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGoalScoreChanged, int32, GoalScore);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCoinChanged, int32, CollectedCoin, int32, TotalCoin);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnExplosionChanged);

UCLASS()
class WAVEGAME_PROJECT_API AWaveGameState : public AGameState
{
	GENERATED_BODY()

public:
	AWaveGameState();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	int32 GetScore() const;
	int32 GetGoalScore() const;
	int32 GetCurrentWaveIndex() const;
	int32 GetCollectedCoinCount() const;
	int32 GetSpawnedCoinCount() const;

public:
	void AddScore(int32 Amount);
	void OnCoinCollected();
	void DestroyAllSpawned();
	void SetTimerHandle();
	void ClearTimerHandle();

public:
	void StartLevel();
	void OnNextLevel();
	void EndLevel();
	void StartWave();
	void OnNextWave();
	void OnGameOver(bool bIsClear);
	void OnOperateObstacle(EObstacleType ObstacleType);
	void NotifyExplosionChanged();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn")
	TArray<TObjectPtr<ABaseItem>> AllItems;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn")
	TArray<TObjectPtr<AFallingObstacle>> FallingObstacles;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn")
	TArray<TObjectPtr<AExpandingObstacle>> ExpandingObstacles;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn")
	TArray<TObjectPtr<AExplosionObstacle>> ExplosionObstacles;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Score")
	int32 Score = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Coin")
	int32 SpawnedCoinCount = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Coin")
	int32 CollectedCoinCount = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Level")
	TArray<FName> LevelNames;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level")
	int32 MaxLevel = 3;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level")
	int32 CurrentLevelIndex = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level|Wave")
	int32 MaxWave = 3;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level|Wave")
	int32 CurrentWaveIndex = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level|Wave")
	int32 WaveGoalScore = 0;

public:
	UPROPERTY(BlueprintAssignable)
	FOnWaveChanged OnWaveChanged;
	UPROPERTY(BlueprintAssignable)
	FOnCoinChanged OnCoinChanged;
	UPROPERTY(BlueprintAssignable)
	FOnScoreChanged OnScoreChanged;
	UPROPERTY(BlueprintAssignable)
	FOnGoalScoreChanged OnGoalScoreChanged;
	UPROPERTY(BlueprintAssignable)
	FOnExplosionChanged OnExplosionChanged;

	FTimerHandle WaveTimerHandle;
	float WaveDuration = 0.f;

	FTimerHandle FallTimerHandle;
	float FallDelay = 5.0f;

	FTimerHandle ExpandTimerHandle;
	float ExpandDelay = 10.0f;

	FTimerHandle ExplosionTimerHandle;
	float ExplosionDelay = 5.0f;

private:
	TObjectPtr<UWaveGameInstance> WaveGameInstance;
};
