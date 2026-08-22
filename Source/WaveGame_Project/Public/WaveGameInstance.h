#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "WaveGameInstance.generated.h"

UCLASS()
class WAVEGAME_PROJECT_API UWaveGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UWaveGameInstance();

public:
	UFUNCTION(BlueprintCallable, Category = "GameData")
	void AddToScore(int32 Amount);

	float GetTotalScore() const;
	int32 GetCurrentLevelIndex() const;
	void SetCurrentLevelIndex(int32 NewCurrentLevelIndex);
	UDataTable* GetCurrentTable() const;
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LevelTable")
	TArray<UDataTable*> LevelTables;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GameData")
	float TotalScore = 0.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GameData")
	int32 CurrentLevelIndex = 0;

};
