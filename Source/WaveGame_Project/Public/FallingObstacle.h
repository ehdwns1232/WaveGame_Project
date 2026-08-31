#pragma once

#include "CoreMinimal.h"
#include "ObstacleBase.h"
#include "FallingObstacle.generated.h"

class UParticleSystem;

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
	virtual void OnObstacleHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Obstacle|Particle")
	TObjectPtr<UParticleSystem> HitParticle;
	
	FTimerHandle FallTimerHandle;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Obstacle|FallImmediately")
	bool bFallImmediately = false;
};
