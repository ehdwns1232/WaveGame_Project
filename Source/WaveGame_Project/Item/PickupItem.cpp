#include "Item/PickupItem.h"
#include "Player/PlayerCharacter.h"
#include "UI/ItemWidget.h"
#include "Components/WidgetComponent.h"
#include "Components/SphereComponent.h"

APickupItem::APickupItem()
{
	ItemName = "DefaultPickup";
	bIsAutoConsumable = false;

	PickupCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Pickup Collision"));
	PickupCollision->SetupAttachment(Scene);
	PickupCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	PickupCollision->InitSphereRadius(PickupDistance);

	PickupCollision->OnComponentBeginOverlap.AddDynamic(this, &APickupItem::OnPickupBeginOverlap);
	PickupCollision->OnComponentEndOverlap.AddDynamic(this, &APickupItem::OnPickupEndOverlap);
}

void APickupItem::ActivateItem(AActor* Activator)
{
	Super::ActivateItem(Activator);
}

float APickupItem::GetPickupDistance() const
{
	return PickupDistance;
}

void APickupItem::OnPickupBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!Other->ActorHasTag(TEXT("PLAYER"))) return;

	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(Other))
	{
		UItemWidget* ItemWidget = Cast<UItemWidget>(ItemWidgetComp->GetUserWidgetObject());
		if (!ItemWidget) return;

		ItemWidget->PlayPickupAnim();
	}
}

void APickupItem::OnPickupEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UItemWidget* ItemWidget = Cast<UItemWidget>(ItemWidgetComp->GetUserWidgetObject());
	if (!ItemWidget) return;

	ItemWidget->StopPickupAnim();
}


