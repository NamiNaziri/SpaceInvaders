// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileLauncher.h"

#include "GameFramework/ProjectileMovementComponent.h"

#include "../ObjectPool/ObjectPoolComponent.h"
#include "../Enemy/ProjectileBaseActor.h"
// Sets default values
AProjectileLauncher::AProjectileLauncher(const FObjectInitializer& ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PoolComponent = CreateDefaultSubobject<UObjectPoolComponent>(TEXT("Object Pool"));


}

// Called when the game starts or when spawned
void AProjectileLauncher::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AProjectileLauncher::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AProjectileLauncher::Launch(FVector Location, FRotator Rotation, FVector LaunchDirection, float Speed)
{
	if (Speed >= 0)
	{
		LaunchDirection *= Speed;
	}
	
	AProjectileBaseActor* Projectile = Cast<AProjectileBaseActor>(PoolComponent->GetNewObjectInstance());
	if (Projectile)
	{
		
		Projectile->SetActorLocation(Location);
		Projectile->SetActorRotation(Rotation);
		UProjectileMovementComponent* PMC = Cast< UProjectileMovementComponent>(Projectile->GetComponentByClass(UProjectileMovementComponent::StaticClass()));
		if (PMC)
		{
			LaunchDirection *= PMC->InitialSpeed;
			PMC->Velocity = LaunchDirection;
		}
		return true;
	}

	return false;
}

TObjectPtr<UObjectPoolComponent> AProjectileLauncher::GetPoolComponent()
{
	return PoolComponent;
}

