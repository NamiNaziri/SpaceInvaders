// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileBaseActor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Engine/DamageEvents.h"
#include "Delegates/Delegate.h"
#include <Components/BoxComponent.h>
#include <Kismet/GameplayStatics.h>



// Sets default values
AProjectileBaseActor::AProjectileBaseActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject <UBoxComponent>(TEXT("Box Component"));
	BoxComponent->InitBoxExtent(FVector(10.f, 10.f, 10.f));
	SetRootComponent(BoxComponent);
	//BoxComponent->SetCollisionProfileName("OverlapProfile");
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AProjectileBaseActor::OnBoxBeginOverlap);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);


	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));
	ProjectileMovement->UpdatedComponent = BoxComponent;
}

// Called when the game starts or when spawned
void AProjectileBaseActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AProjectileBaseActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FOnPoolableObjectReleaseDelegate& AProjectileBaseActor::GetPoolableObjectReleaseDelegate()
{
	return PoolableObjectReleaseDelegate;
}

void AProjectileBaseActor::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning,TEXT("%s"), *(OtherActor->GetDebugName(OtherActor)))
	if (OtherActor == this || OtherActor == GetOwner())
	{
		return;
	}
	
	
	/*
	* Before resetting the enemies that depend on the TakeDamage function, the projectile should be released in order to disable the collision.. 
	* The problem with collision happens when resetting enemies. 
	* If the projectile still has collision, it can collide and kill an enemy while we are resetting the last enemy.
	*/

	if (ExplosionParticleSystem)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionParticleSystem, GetActorTransform());
	}




	// apply point damage
	AController* EventInstigator = nullptr;
	AActor* OwnerProjectile = GetOwner();
	if (OwnerProjectile)
	{
		APawn* OwnerPawn = Cast<APawn>(OwnerProjectile->GetOwner());
		if (OwnerPawn)
		{
			EventInstigator = OwnerPawn->GetController();
		}
	}
	

	FPointDamageEvent PDE;
	OtherActor->TakeDamage(Damage, PDE, EventInstigator, this);
	//TODO perfrom emitter and stuff


	PoolableObjectReleaseDelegate.Broadcast(this);

}

