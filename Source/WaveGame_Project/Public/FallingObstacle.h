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
	virtual bool CanActivate() override;

};
