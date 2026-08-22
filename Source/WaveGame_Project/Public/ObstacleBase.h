#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ObstacleBase.generated.h"

class UBoxComponent;

UCLASS()
class WAVEGAME_PROJECT_API AObstacleBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AObstacleBase();

protected:
	UFUNCTION()
	void OnObstacleHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Obstacle|Components")
	USceneComponent* RootScene;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Obstacle|Components")
	UBoxComponent* BoxCollision;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Obstacle|Components")
	UStaticMeshComponent* StaticMeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float HitDamage;
};
