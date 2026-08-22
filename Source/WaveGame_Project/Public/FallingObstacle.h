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
	void StartFall();

	bool GetIsGround() const;

private:
	bool bIsGround = false;
};
