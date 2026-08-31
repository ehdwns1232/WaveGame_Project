#include "BaseItem.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "MineItem.h"
#include "UI/ItemWidget.h"
#include "Particles/ParticleSystemComponent.h"

ABaseItem::ABaseItem()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Scene);

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	SphereCollision->SetupAttachment(Scene);
	SphereCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(SphereCollision);

	ItemWidgetCollision = CreateDefaultSubobject<USphereComponent>(TEXT("NameRotationCollision"));
	ItemWidgetCollision->SetupAttachment(Scene);
	ItemWidgetCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

	ItemWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("ItemWidgetComponent"));
	ItemWidgetComp->SetupAttachment(Scene);
	ItemWidgetComp->SetWidgetSpace(EWidgetSpace::World);

	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &ABaseItem::OnItemOverlap);
	SphereCollision->OnComponentEndOverlap.AddDynamic(this, &ABaseItem::OnItemEndOverlap);

	ItemWidgetCollision->OnComponentBeginOverlap.AddDynamic(this, &ABaseItem::OnItemWidgetOverlap);
	ItemWidgetCollision->OnComponentEndOverlap.AddDynamic(this, &ABaseItem::OnItemWidgetEndOverlap);
}

void ABaseItem::BeginPlay()
{
	Super::BeginPlay();
	if (ItemWidgetComp)
	{
		ItemWidgetComp->SetVisibility(false);
		SetActorTickEnabled(false);
		ApplyItemWidget();
	}

	// 타이머를 안하고 체크시 Player가 없는 상황이 생겨서 타이머 활용했음 
	FTimerHandle OverlapTimerHandle;
	FTimerDelegate Delegate;
	TWeakObjectPtr<ABaseItem> WeakPtr = this;
	Delegate.BindLambda([WeakPtr]()
		{
			if (!WeakPtr.IsValid()) return;

			ABaseItem* Item = WeakPtr.Get();

			if (!Item->ItemWidgetCollision) return;
			
			TArray<AActor*> OverlappingActors;

			Item->ItemWidgetCollision->GetOverlappingActors(OverlappingActors);

			for (AActor* Actor : OverlappingActors)
			{
				if (Actor && Actor->ActorHasTag("Player"))
				{
					Item->ItemWidgetComp->SetVisibility(true);
					Item->SetActorTickEnabled(true);
					Item->bIsNearPlayer = true;
					break;
				}
			}
		});

	GetWorldTimerManager().SetTimer(OverlapTimerHandle, Delegate, 0.1f, false);
}

void ABaseItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsNearPlayer)
	{
		if (APlayerCameraManager* CameraManager = UGameplayStatics::GetPlayerCameraManager(this, 0))
		{
			if (!ItemWidgetComp) return;

			FVector CameraLocation = CameraManager->GetCameraLocation();
			FVector WidgetLocation = ItemWidgetComp->GetComponentLocation();

			FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(WidgetLocation, CameraLocation);

			ItemWidgetComp->SetWorldRotation(LookAtRotation);
		}
	}
}

void ABaseItem::OnItemOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->ActorHasTag("Player"))
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("Overlap !!!")));
		ActivateItem(OtherActor);
	}
}

void ABaseItem::OnItemEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

void ABaseItem::ActivateItem(AActor* Activator)
{
	UParticleSystemComponent* Particle = nullptr;
	if (PickupParticle)
	{
		Particle = UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			PickupParticle,
			GetActorLocation(),
			GetActorRotation(),
			true
		);
	}
	
	if (PickupSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			GetWorld(),
			PickupSound,
			GetActorLocation()
		);
	}

	if (Particle)
	{
		TWeakObjectPtr<UParticleSystemComponent> WeakPtr = Particle;
		FTimerHandle DestroyParticleTimerHandle;
		GetWorldTimerManager().SetTimer(
			DestroyParticleTimerHandle,
			[WeakPtr]()
			{
				if (WeakPtr.IsValid())
				{
					WeakPtr.Get()->DestroyComponent();
				}
			},
			2.0f,
			false
		);
	}
}

void ABaseItem::DestroyItem()
{
	Destroy();
}

FName ABaseItem::GetItemName() const
{
	return ItemName;
}

void ABaseItem::ApplyItemWidget()
{
	if (!ItemWidgetComp) return;

	if (UItemWidget* ItemWidget = Cast<UItemWidget>(ItemWidgetComp->GetUserWidgetObject()))
	{
		ItemWidget->SetItemInfo(ItemName, ItemIcon);
	}
}

void ABaseItem::OnItemWidgetOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->ActorHasTag("Player"))
	{
		if (!ItemWidgetComp) return;

		bIsNearPlayer = true;
		ItemWidgetComp->SetVisibility(true);
		SetActorTickEnabled(true);
	}
}

void ABaseItem::OnItemWidgetEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor->ActorHasTag("Player"))
	{
		if (!ItemWidgetComp) return;

		bIsNearPlayer = false;
		ItemWidgetComp->SetVisibility(false);
		SetActorTickEnabled(false);
	}
}
