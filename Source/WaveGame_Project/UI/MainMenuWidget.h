#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuWidget.generated.h"

class UTextBlock;
class UButton;
class UWidgetAnimation;

UCLASS()
class WAVEGAME_PROJECT_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

public:
	void UpdateMenuState(bool bIsRestart, bool bIsClear);
	void PlayGameOverAnim();

public:
	UFUNCTION()
	void UpdateTotalScore(int32 TotalScore);
	UFUNCTION()
	void OnStartButtonClick();
	UFUNCTION()
	void OnExitButtonClick();
	UFUNCTION()
	void OnStartButtonHoverAnim();
	UFUNCTION()
	void OnExitButtonHoverAnim();
	UFUNCTION()
	void OnStartButtonUnHoverAnim();
	UFUNCTION()
	void OnExitButtonUnHoverAnim();

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> GameOverText;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> GameOverAnim;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TotalScoreText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> StartButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> StartButtonText;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> StartButtonHoverAnim;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ExitButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ExitButtonText;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> ExitButtonHoverAnim;

private:
	bool bIsGameOver = false;
	bool bIsGameClear = false;
};
