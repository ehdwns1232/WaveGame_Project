#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "MineItem.generated.h"

class USphereComponent;

UCLASS()
class WAVEGAME_PROJECT_API AMineItem : public ABaseItem
{
	GENERATED_BODY()
	
public:
	AMineItem();

public:
	virtual void ActivateItem(AActor* Activator) override;

public:
	void Explode();

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Collision")
	TObjectPtr<USphereComponent> ExplosionCollision;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|UI")
	TObjectPtr<UParticleSystem> ExplosionParticle;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|UI")
	TObjectPtr<USoundBase> ExplosionSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	float ExplosionDelay = 2.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	float ExplosionRadius = 300.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 ExplosionDamage = 30;

private:
	FTimerHandle ExplosionTimerHandle;
	bool bHasExploded = false;
};
