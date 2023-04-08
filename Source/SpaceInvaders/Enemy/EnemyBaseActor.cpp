// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBaseActor.h"

#include <Components/BoxComponent.h>


#include "ProjectileBaseActor.h"

#include "../Launcher/ProjectileLauncher.h"

// Sets default values
AEnemyBaseActor::AEnemyBaseActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Scene Component"));

	BoxComponent = CreateDefaultSubobject <UBoxComponent>(TEXT("Box Component"));
	BoxComponent->InitBoxExtent(FVector(50.f, 50.f, 50.f));
	BoxComponent->SetupAttachment(RootComponent);
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AEnemyBaseActor::OnBoxBeginOverlap);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AEnemyBaseActor::BeginPlay()
{
	Super::BeginPlay();
	InitProjectileLauncher();
}

// Called every frame
void AEnemyBaseActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

const bool& AEnemyBaseActor::IsEnemyEnable()
{
	return this->bIsEnemyEnabled;
}

void AEnemyBaseActor::SetEnemyEnable(bool bIsEnabled)
{
	this->bIsEnemyEnabled = bIsEnabled;
}

void AEnemyBaseActor::Shoot()
{
	if (!ProjectileLauncher)
		return;

	FVector ProjectileLocation = GetActorLocation() - GetActorUpVector() * BoxComponent->GetScaledBoxExtent().Z;
	ProjectileLauncher->Launch(ProjectileLocation, GetActorRotation(), -1 * GetActorUpVector(), -1.f);
}

void AEnemyBaseActor::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == this || OtherActor == GetOwner())
	{
		return;
	}

	//TODO make this better (visually)
	UE_LOG(LogTemp, Warning, TEXT("AEnemyBaseActor::OnBoxBeginOverlap"))
	this->SetActorHiddenInGame(true);
	this->SetActorEnableCollision(false);
	this->SetActorLocation(FVector(0.f, -1000.f, 0.f));
	this->SetEnemyEnable(false);
	OnEnemyHit.Broadcast(this);
}

void AEnemyBaseActor::InitProjectileLauncher()
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

