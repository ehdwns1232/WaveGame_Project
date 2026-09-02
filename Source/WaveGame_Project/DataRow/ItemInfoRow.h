#pragma once

#include "CoreMinimal.h"
#include "ItemInfoRow.generated.h"

USTRUCT(BlueprintType)
struct FItemInfoRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> ItemClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SpawnCount;
};
