#include "SpawnVolume.h"
#include "Components/BoxComponent.h"
#include "CoinItem.h"
#include "MineItem.h"

ASpawnVolume::ASpawnVolume()
{
	PrimaryActorTick.bCanEverTick = false;
	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(RootScene);

	ItemSpawnBox = CreateDefaultSubobject<UBoxComponent>(TEXT("ItemBox Collision"));
	ItemSpawnBox->SetupAttachment(RootScene);
	ItemSpawnBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ItemSpawnBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

	ObstacleTopSpawnBox = CreateDefaultSubobject<UBoxComponent>(TEXT("ObstacleTopBox Collision"));
	ObstacleTopSpawnBox->SetupAttachment(RootScene);
	ObstacleTopSpawnBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ObstacleTopSpawnBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

	ObstacleSideSpawnBox = CreateDefaultSubobject<UBoxComponent>(TEXT("ObstacleSideBox Collision"));
	ObstacleSideSpawnBox->SetupAttachment(RootScene);
	ObstacleSideSpawnBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ObstacleSideSpawnBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
}

void ASpawnVolume::BeginPlay()
{
	Super::BeginPlay();
}

void ASpawnVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

FVector ASpawnVolume::GetRandomPointInVolume(UBoxComponent* Box) const
{
	FVector BoxExtent = Box->GetScaledBoxExtent();
	FVector BoxOrigin = Box->GetComponentLocation();

	return BoxOrigin + FVector(
		FMath::FRandRange(-BoxExtent.X, BoxExtent.X),
		FMath::FRandRange(-BoxExtent.Y, BoxExtent.Y),
		FMath::FRandRange(FMath::Max(0.f, -BoxExtent.Z), BoxExtent.Z));
}

TArray<AActor*> ASpawnVolume::SpawnItem(TSubclassOf<AActor> ItemClass, int32 SpawnCount)
{
	TArray<AActor*> RetArray;
	if (!ItemClass || SpawnCount <= 0) return RetArray;

	for (int32 i = 0; i < SpawnCount; ++i)
	{
		FVector SpawnLocation = GetRandomPointInVolume(ItemSpawnBox);
		if (ItemClass->IsChildOf(AMineItem::StaticClass())) SpawnLocation.Z = 0.f;

		GetWorld()->SpawnActor<AActor>(
			ItemClass,
			SpawnLocation,
			FRotator::ZeroRotator
		);
	}

	return RetArray;
}

TArray<AActor*> ASpawnVolume::SpawnObstacleSide(TSubclassOf<AActor> ObstacleClass, int32 SpawnCount)
{
	TArray<AActor*> RetArray;
	if (!ObstacleClass || SpawnCount <= 0) return RetArray;

	FVector BoxExtent = ObstacleSideSpawnBox->GetScaledBoxExtent();
	FVector BoxOriigin = ObstacleSideSpawnBox->GetComponentLocation();


	FVector SpawnLocation;
	FRotator SpawnRotation;
	for (int32 i = 0; i < SpawnCount; ++i)
	{
		int32 RandXY = FMath::RandRange(0, 1);

		if (RandXY)
		{
			int32 Opposite = FMath::RandRange(0, 1);
			if (Opposite)
			{
				SpawnLocation = FVector(BoxExtent.X, FMath::FRandRange(-BoxExtent.Y, BoxExtent.Y), BoxExtent.Z);
				SpawnRotation = FRotator(0.f, 90.0f, 0.f);
			}
			else
			{
				SpawnLocation = FVector(-BoxExtent.X, FMath::FRandRange(-BoxExtent.Y, BoxExtent.Y), BoxExtent.Z);
				SpawnRotation = FRotator(0.f, -90.f, 0.f);
			}
		}
		else
		{
			int32 Opposite = FMath::RandRange(0, 1);
			if (Opposite)
			{
				SpawnLocation = FVector(FMath::FRandRange(-BoxExtent.X, BoxExtent.X), BoxExtent.Y, BoxExtent.Z);
				SpawnRotation = FRotator(0.f, -180.f, 0.f);
			}
			else
			{
				SpawnLocation = FVector(FMath::FRandRange(-BoxExtent.X, BoxExtent.X), -BoxExtent.Y, BoxExtent.Z);
				SpawnRotation = FRotator(0.f, 0.f, 0.f);
			}
		}

		RetArray.Add(GetWorld()->SpawnActor<AActor>(ObstacleClass, SpawnLocation, SpawnRotation));
	}
	
	return RetArray;
}

TArray<AActor*> ASpawnVolume::SpawnObstacleTop(TSubclassOf<AActor> ObstacleClass, int32 SpawnCount)
{
	TArray<AActor*> RetArray;
	if (!ObstacleClass || SpawnCount <= 0) return RetArray;
	
	for (int32 i = 0; i < SpawnCount; ++i)
	{
		RetArray.Add(GetWorld()->SpawnActor<AActor>(ObstacleClass, GetRandomPointInVolume(ObstacleTopSpawnBox), FRotator::ZeroRotator));
	}

	return RetArray;
}




