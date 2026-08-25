#include "FallingObstacle.h"

AFallingObstacle::AFallingObstacle()
{
	PrimaryActorTick.bCanEverTick = false;
	StaticMeshComp->SetSimulatePhysics(false);
	StaticMeshComp->SetEnableGravity(false);
	StaticMeshComp->SetMobility(EComponentMobility::Movable);
}

void AFallingObstacle::ActivateObstacle()
{
	StaticMeshComp->SetSimulatePhysics(true);
	StaticMeshComp->SetEnableGravity(true);
	bCanActivate = true;
}

bool AFallingObstacle::CanActivate()
{
	return bCanActivate;
}




