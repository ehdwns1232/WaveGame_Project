#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DebuffWidget.generated.h"

class UTextBlock;

UCLASS()
class WAVEGAME_PROJECT_API UDebuffWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

public:
	UFUNCTION()
	void UpdateSlowDebuff(int32 SlowStack);
	UFUNCTION()
	void UpdateReverseDebuff(int32 ReverseStack);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> SlowStackText;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ReverseStackText;
};
