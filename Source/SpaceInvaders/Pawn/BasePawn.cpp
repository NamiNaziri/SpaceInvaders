// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePawn.h"
#include "Components/BoxComponent.h"
#include "Delegates/Delegate.h"
#include "Kismet/GameplayStatics.h"

#include "SpaceInvaders/Enemy/ProjectileBaseActor.h"
#include "SpaceInvaders/Launcher/ProjectileLauncher.h"
#include "SpaceInvaders/GameComponents/Health/HealthComponent.h"

// Sets default values
ABasePawn::ABasePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject <UBoxComponent>(TEXT("Box Component"));
	BoxComponent->InitBoxExtent(FVector(50.f, 50.f, 50.f));
	SetRootComponent(BoxComponent);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

	HealthComponent = CreateDefaultSubobject <UHealthComponent>(TEXT("Health Component"));
	HealthComponent->OnHealthBecomeZero.AddDynamic(this, &ABasePawn::HealthBecomeZero);

	this->OnTakePointDamage.AddDynamic(this, &ABasePawn::TakePointDamage);
}

// Called when the game starts or when spawned
void ABasePawn::BeginPlay()
{
	Super::BeginPlay();
	InitProjectileLauncher();
	PrevLocation = GetActorLocation();
	InitMeshYaw = Mesh->GetRelativeRotation().Yaw;

	if (bDisableAnimation)
	{
		PrimaryActorTick.bCanEverTick = false;
	}
}

// Called every frame
void ABasePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector VelocityDirection = (GetActorLocation() - PrevLocation).GetSafeNormal();
	float HorizontalDirection = VelocityDirection.Y;

	float MeshTargetYaw = ( - 1 * HorizontalDirection * TargetAngleDifference) + InitMeshYaw;

	FRotator MeshCurrentRotation = Mesh->GetRelativeRotation();
	FRotator MeshTargetRotation = FRotator(MeshCurrentRotation.Pitch, MeshTargetYaw, MeshCurrentRotation.Roll);
	FRotator MeshNewRotation = FMath::RInterpTo(MeshCurrentRotation, MeshTargetRotation, DeltaTime, MeshRotationRate);

	Mesh->SetRelativeRotation(MeshNewRotation);

	PrevLocation = GetActorLocation();
}

// Called to bind functionality to input
void ABasePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ABasePawn::TakePointDamage(AActor* DamagedActor, float Damage, AController* InstigatedBy, FVector HitLocation, UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const UDamageType* DamageType, AActor* DamageCauser)
{
}

void ABasePawn::HealthBecomeZero(AActor* OwnerActor)
{
	if (ExplosionParticleSystem)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionParticleSystem, GetActorTransform());
	}
}

void ABasePawn::InitProjectileLauncher()
{
	if (ProjectileLauncherClass)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;

		// Spawn the actor
		ProjectileLauncher = GetWorld()->SpawnActor<AProjectileLauncher>(
			ProjectileLauncherClass,
			GetActorLocation(),
			GetActorRotation(),
			SpawnParams);
		if (ProjectileLauncher)
			ProjectileLauncher->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
	}
}

