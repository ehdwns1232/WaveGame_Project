#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "WaveGameInstance.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTotalScoreChanged, int32, TotalScore);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLevelChanged, int32, CurLevel);

UCLASS()
class WAVEGAME_PROJECT_API UWaveGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UWaveGameInstance();

public:
	UFUNCTION(BlueprintCallable, Category = "GameData")
	void AddToScore(int32 Amount);

	int32 GetTotalScore() const;
	void SetTotalScore(int32 Amount);
	int32 GetCurrentLevelIndex() const;
	void SetCurrentLevelIndex(int32 NewCurrentLevelIndex);
	UDataTable* GetCurrentTable() const;
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LevelTable")
	TArray<TObjectPtr<UDataTable>> LevelTables;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GameData")
	int32 TotalScore = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GameData")
	int32 CurrentLevelIndex = 0;

public:
	UPROPERTY(BlueprintAssignable)
	FOnTotalScoreChanged OnTotalScoreChanged;
	UPROPERTY(BlueprintAssignable)
	FOnLevelChanged OnLevelChanged;
};
