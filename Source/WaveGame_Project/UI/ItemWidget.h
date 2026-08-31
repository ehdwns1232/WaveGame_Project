#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemWidget.generated.h"

class UTextBlock;
class UImage;

UCLASS()
class WAVEGAME_PROJECT_API UItemWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetItemInfo(FName ItemName, UTexture2D* ItemIcon);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> NameText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> IconImage;
};
