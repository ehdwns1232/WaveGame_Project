#include "FallingObstacle.h"
#include "Kismet/GameplayStatics.h"

AFallingObstacle::AFallingObstacle()
{
	PrimaryActorTick.bCanEverTick = false;
	StaticMeshComp->SetSimulatePhysics(false);
	StaticMeshComp->SetEnableGravity(false);
	StaticMeshComp->SetMobility(EComponentMobility::Movable);

	HitParticle = nullptr;
}

void AFallingObstacle::ActivateObstacle()
{
	StaticMeshComp->SetSimulatePhysics(true);
	StaticMeshComp->SetEnableGravity(true);
	bCanActivate = true;
}

void AFallingObstacle::BeginPlay()
{
	Super::BeginPlay();

	TWeakObjectPtr<AFallingObstacle> WeakPtr = this;
	GetWorldTimerManager().SetTimer(FallTimerHandle, [WeakPtr]() { if (WeakPtr.IsValid()) { if (WeakPtr.Get()->bFallImmediately)WeakPtr->ActivateObstacle(); } }, 0.1f, false);
}

void AFallingObstacle::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	GetWorldTimerManager().ClearTimer(FallTimerHandle);

	Super::EndPlay(EndPlayReason);
}

void AFallingObstacle::OnObstacleHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!bCanApplyDamage) return;

	Super::OnObstacleHit(HitComponent, OtherActor, OtherComp, NormalImpulse, Hit);
	if (HitParticle)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticle, HitComponent->GetComponentLocation(), HitComponent->GetComponentRotation(), true);
	}
}





