#include "ExplosionObstacle.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "WaveGameState.h"

AExplosionObstacle::AExplosionObstacle()
{
	StaticMeshComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

	ExplosionCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Explosion Collision"));
	ExplosionCollision->SetupAttachment(RootComponent);
	ExplosionCollision->InitSphereRadius(ExplosionRadius);
	ExplosionCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

	BoxCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &AExplosionObstacle::OnExplosionBeginOverlap);
}

void AExplosionObstacle::OnExplosionBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->ActorHasTag(TEXT("Player")))
	{
		ActivateObstacle();
	}
}

void AExplosionObstacle::ActivateObstacle()
{
	if (AWaveGameState* WaveGameState = Cast<AWaveGameState>(GetWorld()->GetGameState()))
	{
		WaveGameState->NotifyExplosionChanged();
	}
	UParticleSystemComponent* Particle = nullptr;
	if (ActivateParticle)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ActivateParticle, ExplosionCollision->GetComponentLocation(), ExplosionCollision->GetComponentRotation(), true);
	}
	if (ActivateSound)
	{
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), ActivateSound, GetActorLocation());
	}

	bCanActivate = true;
	TArray<AActor*> OverlappingActors;
	ExplosionCollision->GetOverlappingActors(OverlappingActors);
	for (AActor* Actor : OverlappingActors)
	{
		if (Actor->ActorHasTag(TEXT("Player")))
		{
			UGameplayStatics::ApplyDamage(Actor, ExplosionDamage, nullptr, this, UDamageType::StaticClass());
		}
	}
	Destroy();
}


