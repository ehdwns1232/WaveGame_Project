#include "ObstacleBase.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

AObstacleBase::AObstacleBase()
{
	PrimaryActorTick.bCanEverTick = false;

	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(RootScene);

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("CapsuleCollision"));
	BoxCollision->SetupAttachment(RootScene);
	BoxCollision->SetNotifyRigidBodyCollision(true);
	BoxCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BoxCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	BoxCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	
	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMeshComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	StaticMeshComp->SetupAttachment(RootScene);
	StaticMeshComp->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	StaticMeshComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);

	BoxCollision->OnComponentHit.AddDynamic(this, &AObstacleBase::OnObstacleHit);

	HitDamage = 30.0f;
}

bool AObstacleBase::CanActivate()
{
	return bCanActivate;
}
void AObstacleBase::ActivateObstacle()
{
}

void AObstacleBase::OnObstacleHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor && OtherActor->ActorHasTag(FName(TEXT("Player"))))
	{
		UGameplayStatics::ApplyDamage(OtherActor, HitDamage, nullptr, this, UDamageType::StaticClass());
	}
}


