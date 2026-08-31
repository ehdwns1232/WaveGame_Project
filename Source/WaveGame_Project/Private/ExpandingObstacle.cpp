#include "ExpandingObstacle.h"
#include "Components/BoxComponent.h"
#include "PlayerCharacter.h"

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

	if (!bCanActivate) return;

	Alpha += (ExpandSpeed / MaxExpand) * DeltaTime;
	if (Alpha >= 1.0f) { Alpha = 1.0f; ExpandSpeed *= -1; }
	else if (Alpha <= 0.f) { Alpha = 0.f; ExpandSpeed *= -1; bCanActivate = false; }

	float FinalAlpha = CalculateAlpha();
	RootComponent->SetRelativeScale3D(FMath::Lerp(StartScale, EndScale, FinalAlpha));
}

void AExpandingObstacle::ActivateObstacle()
{
	Alpha = 0.f;
	bCanActivate = true;
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

void AExpandingObstacle::OnObstacleHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!bCanApplyDamage) return;

	Super::OnObstacleHit(HitComponent, OtherActor, OtherComp, NormalImpulse, Hit);
	
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn()))
	{
		FVector PushDir = PlayerCharacter->GetActorLocation() - GetActorLocation();
		PushDir.Normalize();

		FVector PushVelocity = PushDir * PushPower;

		PlayerCharacter->LaunchCharacter(PushVelocity, true, true);
	}
}

