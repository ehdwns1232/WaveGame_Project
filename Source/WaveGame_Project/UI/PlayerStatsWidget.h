#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerStatsWidget.generated.h"

class UTextBlock;
class UProgressBar;

UCLASS()
class WAVEGAME_PROJECT_API UPlayerStatsWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

public:
	UFUNCTION()
	void UpdateHP(float CurHP, float MaxHP);
	UFUNCTION()
	void UpdateSprint(float CurSprintRate, float MaxSprintRate);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> HPText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> HPBar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> SprintBar;
};
