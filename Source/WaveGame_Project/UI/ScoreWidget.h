#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ScoreWidget.generated.h"

class UTextBlock;

UCLASS()
class WAVEGAME_PROJECT_API UScoreWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

public:
	UFUNCTION()
	void UpdateScore(int32 CurScore);
	UFUNCTION()
	void UpdateGoalScore(int32 GoalScore);
	UFUNCTION()
	void UpdateTotalScore(int32 TotalScore);
protected:

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ScoreText;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> GoalScoreText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TotalScoreText;

};
