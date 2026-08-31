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
	virtual void OnObstacleHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
public:
	bool GetCanActivate();
	virtual void ActivateObstacle();
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Obstacle|Components")
	TObjectPtr<USceneComponent> RootScene;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Obstacle|Components")
	TObjectPtr<UBoxComponent> BoxCollision;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Obstacle|Components")
	TObjectPtr<UStaticMeshComponent> StaticMeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float HitDamage = 30.0f;

protected:
	bool bCanActivate = false;
	bool bCanApplyDamage = true;

private:
	FTimerHandle ResetApplyDamageTimerHandle;
};
