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
	
	virtual void ActivateObstacle() override;
	virtual bool CanActivate() override;
	float CalculateAlpha();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Expand")
	EEasingMode EasingMode = EEasingMode::EaseIn;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Exapand", meta = (ClampMin = 1.0f))
	float ExpandSpeed = 3.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Expand", meta = (ClampMin = 1.0f))
	float MaxExpand = 10.0f;

private:
	float Alpha = 0.0f;
	FVector StartScale = FVector(1.0f, 1.0f, 1.0f);
	FVector EndScale = FVector(1.0f, MaxExpand, 1.0f);

};
