#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "WaveGameState.generated.h"

class AFallingObstacle;
class AExpandingObstacle;

UENUM(BlueprintType)
enum class EObstacleType : uint8
{
	Fall,
	Expand
};

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
	UFUNCTION(BlueprintPure, Category = "Score")
	int32 GetScore() const;
	UFUNCTION(BlueprintCallable, Category = "Score")
	void AddScore(int32 Amount);

	void OnCoinCollected();
	void DestroyAllSpawned();
public:
	void StartLevel();
	void OnNextLevel();
	void EndLevel();
	void StartWave();
	void OnNextWave();
	UFUNCTION(BlueprintCallable, Category = "Level")
	void OnGameOver();
	void OnOperateObstacle(EObstacleType ObstacleType);

	void SetTimerHandle();
	void ClearTimerHandle();

	void UpdateHUD();
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn")
	TArray<AFallingObstacle*> FallingObstacles;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn")
	TArray<AExpandingObstacle*> ExpandingObstacles;
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
private:
	FTimerHandle UpdateHUDTimerHandle;

	FTimerHandle WaveTimerHandle;
	float WaveDuration = 0.f;

	FTimerHandle FallTimerHandle;
	float FallDelay = 5.0f;

	FTimerHandle ExpandTimerHandle;
	float ExpandDelay = 10.0f;
};
