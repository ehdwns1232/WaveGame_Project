#include "ObstacleBase.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

AObstacleBase::AObstacleBase()
{
	PrimaryActorTick.bCanEverTick = false;

	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(RootScene);

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMeshComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	StaticMeshComp->SetupAttachment(RootScene);
	StaticMeshComp->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	StaticMeshComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("CapsuleCollision"));
	BoxCollision->SetupAttachment(StaticMeshComp);
	BoxCollision->SetNotifyRigidBodyCollision(true);
	BoxCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BoxCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	BoxCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);

	BoxCollision->OnComponentHit.AddDynamic(this, &AObstacleBase::OnObstacleHit);
}

bool AObstacleBase::GetCanActivate()
{
	return bCanActivate;
}
void AObstacleBase::ActivateObstacle()
{
}

void AObstacleBase::OnObstacleHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (bCanApplyDamage && OtherActor && OtherActor->ActorHasTag(FName(TEXT("Player"))))
	{
		UParticleSystemComponent* Particle = nullptr;
		if (ActivateParticle)
		{
			Particle = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ActivateParticle, GetActorLocation(), GetActorRotation(), true);
		}

		if (ActivateSound)
		{
			UGameplayStatics::SpawnSoundAtLocation(GetWorld(), ActivateSound, GetActorLocation());
		}

		bCanApplyDamage = false;
		UGameplayStatics::ApplyDamage(OtherActor, HitDamage, nullptr, this, UDamageType::StaticClass());
		TWeakObjectPtr<AObstacleBase> WeakPtr = this;
		GetWorldTimerManager().SetTimer(ResetApplyDamageTimerHandle, [WeakPtr]() { if (WeakPtr.IsValid()){ WeakPtr.Get()->bCanApplyDamage = true; } }, 1.2f, false);
	}
}


