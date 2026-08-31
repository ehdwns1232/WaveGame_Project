#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUDWidget.generated.h"

class UTextBlock;
class UTimeWidget;
class UPlayerStatsWidget;
class UPlayerDirectionWidget;
class UScoreWidget;
class UDebuffWidget;

UCLASS()
class WAVEGAME_PROJECT_API UHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

public:
	UFUNCTION()
	void UpdateLevel(int32 CurLevel);
	UFUNCTION()
	void UpdateWave(int32 CurWave);
	UFUNCTION()
	void UpdateCoin(int32 CollectedCoinCount, int32 TotalCoinCount);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> LevelText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> WaveText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> CoinText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTimeWidget> TimeWidget;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UPlayerStatsWidget> PlayerStatsWidget;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UPlayerDirectionWidget> PlayerDirectionWidget;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UDebuffWidget> DebuffWidget;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UScoreWidget> ScoreWidget;
};
