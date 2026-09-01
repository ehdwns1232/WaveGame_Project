#pragma once

#include "CoreMinimal.h"
#include "ObstacleBase.h"
#include "FallingObstacle.generated.h"

UCLASS()
class WAVEGAME_PROJECT_API AFallingObstacle : public AObstacleBase
{
	GENERATED_BODY()

public:
	AFallingObstacle();
	
public:
	virtual void ActivateObstacle() override;
	
protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

protected:
	FTimerHandle FallTimerHandle;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Obstacle|FallImmediately")
	bool bFallImmediately = false;
};
