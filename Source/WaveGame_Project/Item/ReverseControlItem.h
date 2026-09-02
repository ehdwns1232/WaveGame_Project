#pragma once

#include "CoreMinimal.h"
#include "Item/BaseItem.h"
#include "ReverseControlItem.generated.h"

UCLASS()
class WAVEGAME_PROJECT_API AReverseControlItem : public ABaseItem
{
	GENERATED_BODY()
	
public:
	AReverseControlItem();

public:
	virtual void ActivateItem(AActor* Activator) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ReverseTime")
	float ReverseTime = 10.0f;
};
