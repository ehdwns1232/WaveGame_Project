#pragma once

#include "CoreMinimal.h"
#include "PickupItem.h"
#include "StaminaItem.generated.h"

UCLASS()
class WAVEGAME_PROJECT_API AStaminaItem : public APickupItem
{
	GENERATED_BODY()
	
public:
	AStaminaItem();

public:
	virtual void ActivateItem(AActor* Activator) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	float StaminaAmount = 2.0f;
};
