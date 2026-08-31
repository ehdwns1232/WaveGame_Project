#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerDirectionWidget.generated.h"

class UImage;
class UWidgetAnimation;

UCLASS()
class WAVEGAME_PROJECT_API UPlayerDirectionWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

public:
	UFUNCTION()
	void UpdateDirection(FVector2D MoveInput);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> DirUp;
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> DirUpAnim;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> DirDown;
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> DirDownAnim;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> DirRight;
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> DirRightAnim;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> DirLeft;
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> DirLeftAnim;

private:
	FVector2D PreviousMoveInput = FVector2D::ZeroVector;
};
