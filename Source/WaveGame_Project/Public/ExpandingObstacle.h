#pragma once

#include "CoreMinimal.h"
#include "ObstacleBase.h"
#include "ExpandingObstacle.generated.h"

class UArrowComponent;

UENUM(BlueprintType)
enum class EEasingMode : uint8
{
	EaseIn		UMETA(DisplayName = "1. EaseIn"),
	EaseOut		UMETA(DisplayName = "2. EaseOut"),
	EaseInOut	UMETA(DisplayName = "3. EaseInOut")
};

UCLASS()
class WAVEGAME_PROJECT_API AExpandingObstacle : public AObstacleBase
{
	GENERATED_BODY()
	
public:
	AExpandingObstacle();

protected:
	virtual void BeginPlay() override;
public:
	virtual void Tick(float DeltaTime) override;
	void StartExpand();
	float CalculateAlpha();

	bool GetIsExpanding();
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Expand")
	EEasingMode EasingMode = EEasingMode::EaseIn;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Exapand", meta = (ClampMin = 1.f))
	float ExpandSpeed = 3.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Expand", meta = (ClampMin = 1.f))
	float MaxExpand = 10.f;

private:
	float Alpha = 0.0f;
	FVector StartScale = FVector(1.f, 0.01f, 1.f);
	FVector EndScale = FVector(1.f, MaxExpand, 1.f);

	bool bIsExpanding = false;
};
