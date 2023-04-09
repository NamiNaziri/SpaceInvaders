// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBasePawn.h"
#include <Components/BoxComponent.h>
#include "ProjectileBaseActor.h"
#include "../GameComponents/Health/HealthComponent.h"	
#include "../Launcher/ProjectileLauncher.h"

// Sets default values
AEnemyBasePawn::AEnemyBasePawn()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Scene Component"));
	Super::BoxComponent->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AEnemyBasePawn::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AEnemyBasePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

const bool& AEnemyBasePawn::IsEnemyEnable()
{
	return this->bIsEnemyEnabled;
}

void AEnemyBasePawn::SetEnemyEnable(bool bIsEnabled)
{
	this->bIsEnemyEnabled = bIsEnabled;
}

void AEnemyBasePawn::Shoot()
{
	if (!ProjectileLauncher)
		return;

	FVector ProjectileLocation = GetActorLocation() - GetActorUpVector() * BoxComponent->GetScaledBoxExtent().Z;
	ProjectileLauncher->Launch(ProjectileLocation, GetActorRotation(), -1 * GetActorUpVector(), -1.f);
}


void AEnemyBasePawn::TakePointDamage(AActor* DamagedActor, float Damage, AController* InstigatedBy, FVector HitLocation, UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const UDamageType* DamageType, AActor* DamageCauser)
{
	//TODO make this better (visually)

	HealthComponent->DecreaseHealth(Damage);
	OnEnemyHit.Broadcast(this);


}

void AEnemyBasePawn::HealthBecomeZero(AActor* OwnerActor)
{
	//TODO make this better (visually)
	this->SetActorHiddenInGame(true);
	this->SetActorEnableCollision(false);
	this->SetActorLocation(FVector(0.f, -1000.f, 0.f));
	this->SetEnemyEnable(false);
}
