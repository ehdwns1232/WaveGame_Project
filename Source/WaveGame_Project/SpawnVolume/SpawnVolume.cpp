#include "SpawnVolume/SpawnVolume.h"
#include "Components/BoxComponent.h"
#include "Item/CoinItem.h"
#include "Item/MineItem.h"

ASpawnVolume::ASpawnVolume()
{
	PrimaryActorTick.bCanEverTick = false;
	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(RootScene);

	BottomSpawnBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BottomBox Collision"));
	BottomSpawnBox->SetupAttachment(RootScene);
	BottomSpawnBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BottomSpawnBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

	TopSpawnBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TopSpawnBox Collision"));
	TopSpawnBox->SetupAttachment(RootScene);
	TopSpawnBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	TopSpawnBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

	SideSpawnBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SideSpawnBox Collision"));
	SideSpawnBox->SetupAttachment(RootScene);
	SideSpawnBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SideSpawnBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
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
		FVector SpawnLocation = GetRandomPointInVolume(BottomSpawnBox);
		if (ItemClass->IsChildOf(AMineItem::StaticClass())) SpawnLocation.Z = 0.f;

		RetArray.Add(GetWorld()->SpawnActor<AActor>(
			ItemClass,
			SpawnLocation,
			FRotator::ZeroRotator
		));
	}

	return RetArray;
}

TArray<AActor*> ASpawnVolume::SpawnObstacleSide(TSubclassOf<AActor> ObstacleClass, int32 SpawnCount)
{
	TArray<AActor*> RetArray;
	if (!ObstacleClass || SpawnCount <= 0) return RetArray;

	FVector BoxExtent = SideSpawnBox->GetScaledBoxExtent();
	FVector BoxOriigin = SideSpawnBox->GetComponentLocation();


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
		RetArray.Add(GetWorld()->SpawnActor<AActor>(ObstacleClass, GetRandomPointInVolume(TopSpawnBox), FRotator::ZeroRotator));
	}

	return RetArray;
}

TArray<AActor*> ASpawnVolume::SpawnObstacleBottom(TSubclassOf<AActor> ObstacleClass, int32 SpawnCount)
{
	TArray<AActor*> RetArray;
	if (!ObstacleClass || SpawnCount <= 0) return RetArray;

	for (int32 i = 0; i < SpawnCount; ++i)
	{
		FVector SpawnLocation = GetRandomPointInVolume(BottomSpawnBox);
		SpawnLocation.Z = 0.0f;

		RetArray.Add(GetWorld()->SpawnActor<AActor>(
			ObstacleClass,
			SpawnLocation,
			FRotator::ZeroRotator
		));
	}

	return RetArray;
}

