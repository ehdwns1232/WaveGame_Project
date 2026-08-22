#include "FallingObstacle.h"

AFallingObstacle::AFallingObstacle()
{
	PrimaryActorTick.bCanEverTick = false;
	StaticMeshComp->SetSimulatePhysics(false);
	StaticMeshComp->SetEnableGravity(false);
}

void AFallingObstacle::StartFall()
{
	StaticMeshComp->SetSimulatePhysics(true);
	StaticMeshComp->SetEnableGravity(true);
	bIsGround = true;
	UE_LOG(LogTemp, Warning, TEXT("StartFall End"));
}

bool AFallingObstacle::GetIsGround() const
{
	return bIsGround;
}


