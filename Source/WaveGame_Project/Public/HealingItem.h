#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "HealingItem.generated.h"

UCLASS()
class WAVEGAME_PROJECT_API AHealingItem : public ABaseItem
{
	GENERATED_BODY()
	
public:
	AHealingItem();

public:
	virtual void ActivateItem(AActor* Activator) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 HealAmount = 20;
};
