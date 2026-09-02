#pragma once

#include "CoreMinimal.h"
#include "Item/BaseItem.h"
#include "SlowingItem.generated.h"

UCLASS()
class WAVEGAME_PROJECT_API ASlowingItem : public ABaseItem
{
	GENERATED_BODY()
	
public:
	ASlowingItem();

public:
	virtual void ActivateItem(AActor* Activator) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SlowTime")
	float SlowTime = 5.0f;
};
