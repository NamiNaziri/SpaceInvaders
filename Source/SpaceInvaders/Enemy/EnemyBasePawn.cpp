// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBasePawn.h"
#include "Components/BoxComponent.h"
#include "Engine/DamageEvents.h"

#include "Kismet/GameplayStatics.h"

#include "SpaceInvaders/GameComponents/Health/HealthComponent.h"	
#include "SpaceInvaders/Launcher/ProjectileLauncher.h"
#include "SpaceInvaders/Player/PlayerBaseController.h"
#include "SpaceInvaders/Game/CoreGameState.h"
#include "SpaceInvaders/Destructibles/DestructibleActor.h"
#include "SpaceInvaders/Launcher/ProjectileBaseActor.h"

// Sets default values
AEnemyBasePawn::AEnemyBasePawn()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Scene Component"));
	Super::BoxComponent->SetupAttachment(RootComponent);

	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AEnemyBasePawn::OnBoxBeginOverlap);
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
	LastInstigator = InstigatedBy;
	HealthComponent->DecreaseHealth(Damage);
}

void AEnemyBasePawn::HealthBecomeZero(AActor* OwnerActor)
{
	if (IsEnemyEnable())
	{
		Super::HealthBecomeZero(OwnerActor);

		//TODO make this better (visually)

		this->SetActorHiddenInGame(true);
		this->SetActorEnableCollision(false);
		this->SetEnemyEnable(false);
		BoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		// Add score to the last instigator if exist
		APlayerBaseController* PBC = Cast<APlayerBaseController>(LastInstigator);
		if (PBC)
		{
			PBC->AddScore(PointsPerKill);
		}

		SetEnemyEnable(false);

		if (ExplosionSoundCue)
		{
			UGameplayStatics::PlaySound2D(GetWorld(), ExplosionSoundCue);
		}

		OnEnemyDestroyed.Broadcast(this);
	}

}

void AEnemyBasePawn::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == this || OtherActor == GetOwner())
	{
		return;
	}

	ADestructibleActor* DestructibleActor = Cast<ADestructibleActor >(OtherActor);
	if (DestructibleActor)
	{
		FPointDamageEvent PDE;
		PDE.HitInfo = SweepResult;
		OtherActor->TakeDamage(1000000.f, PDE, nullptr, this); // make sure that the destructible actor will die.

		// Destroy self.
		HealthBecomeZero(this);
	}
}

void AEnemyBasePawn::ResetEnemy()
{
	this->SetActorHiddenInGame(false);
	this->SetActorEnableCollision(true);
	this->SetEnemyEnable(true);
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	SetEnemyEnable(true);
	HealthComponent->ResetHealth();
}
