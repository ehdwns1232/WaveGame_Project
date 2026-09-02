#pragma once

#include "CoreMinimal.h"
#include "Item/BaseItem.h"
#include "PickupItem.generated.h"

class USphereComponent;

UCLASS()
class WAVEGAME_PROJECT_API APickupItem : public ABaseItem
{
	GENERATED_BODY()
	
public:
	APickupItem();

public:
	virtual void ActivateItem(AActor* Activator) override;
	
public:
	float GetPickupDistance() const;
	UFUNCTION()
	void OnPickupBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnPickupEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
	UPROPERTY(VisibleAnywhere, Category = "Item|Component")
	TObjectPtr<USphereComponent> PickupCollision;

private:
	float PickupDistance = 200.0f;
};
