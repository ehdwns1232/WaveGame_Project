#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseItem.generated.h"

class USphereComponent;
class UWidgetComponent;

UCLASS()
class WAVEGAME_PROJECT_API ABaseItem : public AActor
{
	GENERATED_BODY()
	
public:	
	ABaseItem();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float Deltatime) override;
	UFUNCTION()
	virtual void OnItemOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);
	UFUNCTION()
	virtual void OnItemEndOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);
	virtual void ActivateItem(AActor* Activator);
	virtual void DestroyItem();
	FName GetItemType() const;

	void ApplyNameWidget();
	UFUNCTION()
	void OnItemWidgetOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnItemWidgetEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|Component")
	USceneComponent* Scene;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|Component")
	USphereComponent* SphereCollision;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|Component")
	UStaticMeshComponent* StaticMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|Component")
	USphereComponent* ItemWidgetCollision;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|UI")
	UWidgetComponent* ItemWidget;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|UI")
	UTexture2D* ItemIcon;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|UI")
	UParticleSystem* PickupParticle;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|UI")
	USoundBase* PickupSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FName ItemType;
};
