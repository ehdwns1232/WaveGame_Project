#pragma once

#include "CoreMinimal.h"
#include "WaveDataRow.generated.h"

USTRUCT(BlueprintType)
struct FSpawnInfoPair
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> SpawnClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SpawnCount;
};


USTRUCT(BlueprintType)
struct FWaveDataRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FSpawnInfoPair> SpawnItemInfo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WaveTimeDuration;

};
