#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemWidget.generated.h"

class UTextBlock;
class UImage;
class UWidgetAnimation;

UCLASS()
class WAVEGAME_PROJECT_API UItemWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetItemInfo(FName ItemName, UTexture2D* ItemIcon, bool bIsAutoConsumable);
	void SetPickupTextVisible(bool bIsAutoConsumable);
	void PlayPickupAnim();
	void StopPickupAnim();
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> NameText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> IconImage;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> PickupText;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> PickupAnim;
};
