#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUDWidget.generated.h"

class UGameInfoWidget;
class UTimeWidget;
class UPlayerStatsWidget;
class UPlayerDirectionWidget;
class UScoreWidget;
class UDebuffWidget;

UCLASS()
class WAVEGAME_PROJECT_API UHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UGameInfoWidget> GameInfoWidget;

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
