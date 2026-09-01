#pragma once

#include "CoreMinimal.h"
#include "ObstacleBase.h"
#include "ExplosionObstacle.generated.h"

class USphereComponent;

UCLASS()
class WAVEGAME_PROJECT_API AExplosionObstacle : public AObstacleBase
{
	GENERATED_BODY()
	
public:
	AExplosionObstacle();

public:
	UFUNCTION()
	void OnExplosionBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	virtual void ActivateObstacle() override;

public:
	void Explode();

public:
	UPROPERTY(VisibleAnywhere, Category = "Obstacle|Component")
	TObjectPtr<USphereComponent> ExplosionCollision;

	float ExplosionRadius = 1000.0f;
	float ExplosionDamage = 50.0f;
};
