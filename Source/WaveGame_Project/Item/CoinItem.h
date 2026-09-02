#pragma once

#include "CoreMinimal.h"
#include "Item/BaseItem.h"
#include "CoinItem.generated.h"

UCLASS()
class WAVEGAME_PROJECT_API ACoinItem : public ABaseItem
{
	GENERATED_BODY()
	
public:
	ACoinItem();

public:
	virtual void ActivateItem(AActor* Activator) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 PointValue = 0;
};
