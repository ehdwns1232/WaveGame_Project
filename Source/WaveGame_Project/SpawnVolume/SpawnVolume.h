#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DataRow/WaveDataRow.h"
#include "SpawnVolume.generated.h"

class UBoxComponent;

UCLASS()
class WAVEGAME_PROJECT_API ASpawnVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	ASpawnVolume();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

public:
	FVector GetRandomPointInVolume(UBoxComponent* Box) const;
	TArray<AActor*> SpawnItem(TSubclassOf<AActor> ItemClass, int32 SpawnCount);
	TArray<AActor*> SpawnObstacleSide(TSubclassOf<AActor> ObstacleClass, int32 SpawnCount);
	TArray<AActor*> SpawnObstacleTop(TSubclassOf<AActor> ObstacleClass, int32 SpawnCount);
	TArray<AActor*> SpawnObstacleBottom(TSubclassOf<AActor> ObstacleClass, int32 SpawnCount);

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning")
	TObjectPtr<USceneComponent> RootScene;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning")
	TObjectPtr<UBoxComponent> BottomSpawnBox;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning")
	TObjectPtr<UBoxComponent> TopSpawnBox;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning")
	TObjectPtr<UBoxComponent> SideSpawnBox;
};
