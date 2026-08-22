#include "ExpandingObstacle.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"

AExpandingObstacle::AExpandingObstacle()
{
	PrimaryActorTick.bCanEverTick = true;

	BoxCollision->SetRelativeLocation(FVector(0.f, 50.f, 0.f));
	StaticMeshComp->SetRelativeLocation(FVector(0.f, 50.f, 0.f));
}

void AExpandingObstacle::BeginPlay()
{
	Super::BeginPlay();

}

void AExpandingObstacle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bIsExpanding) return;

	Alpha += (ExpandSpeed / MaxExpand) * DeltaTime;
	if (Alpha >= 1.f) { Alpha = 1.f; ExpandSpeed *= -1; }
	else if (Alpha <= 0.f) { Alpha = 0.f; ExpandSpeed *= -1; bIsExpanding = false; }

	float FinalAlpha = CalculateAlpha();
	RootComponent->SetRelativeScale3D(FMath::Lerp(StartScale, EndScale, FinalAlpha));
}

void AExpandingObstacle::StartExpand()
{
	Alpha = 0.f;
	bIsExpanding = true;
	UE_LOG(LogTemp, Warning, TEXT("StartExpand End"));
}

float AExpandingObstacle::CalculateAlpha()
{
	switch (EasingMode)
	{
	case EEasingMode::EaseIn:
		return (1.f - FMath::Cos(Alpha * HALF_PI));
	case EEasingMode::EaseOut:
		return FMath::Sin(Alpha * HALF_PI);
	case EEasingMode::EaseInOut:
		return (1.f - FMath::Cos(Alpha * PI)) * 0.5f;
	default:
		break;
	}
	
	return Alpha;
}

bool AExpandingObstacle::GetIsExpanding()
{
	return bIsExpanding;
}


