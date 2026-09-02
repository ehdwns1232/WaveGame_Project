#include "Obstacle/FallingObstacle.h"
#include "Kismet/GameplayStatics.h"

AFallingObstacle::AFallingObstacle()
{
	PrimaryActorTick.bCanEverTick = false;
	StaticMeshComp->SetSimulatePhysics(false);
	StaticMeshComp->SetEnableGravity(false);
	StaticMeshComp->SetMobility(EComponentMobility::Movable);

}

void AFallingObstacle::ActivateObstacle()
{
	Super::ActivateObstacle();
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






