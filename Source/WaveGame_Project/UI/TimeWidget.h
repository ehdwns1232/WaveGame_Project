#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TimeWidget.generated.h"

class UTextBlock;
class UWidgetAnimation;
class AWaveGameState;

UCLASS()
class WAVEGAME_PROJECT_API UTimeWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

public:
	void UpdateRemainingTime();

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TimeText;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> TimeAnim;

private:
	UPROPERTY()
	TObjectPtr<AWaveGameState> WaveGameState;

	FTimerHandle UpdateRemainingTimeHandle;
};
