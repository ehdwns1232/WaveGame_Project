#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameInfoWidget.generated.h"

class UTextBlock;
class UWidgetAnimation;

UCLASS()
class WAVEGAME_PROJECT_API UGameInfoWidget : public UUserWidget
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
	UFUNCTION()
	void ShowExplosionEvent();
	void HideExplosionEvent();
public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> LevelText;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> WaveText;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> CoinText;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ExplosionText;
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> ExplosionAnim;
};
