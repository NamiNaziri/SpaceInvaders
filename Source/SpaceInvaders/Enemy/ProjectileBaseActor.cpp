// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileBaseActor.h"
#include "GameFramework/ProjectileMovementComponent.h"

#include <Components/BoxComponent.h>

// Sets default values
AProjectileBaseActor::AProjectileBaseActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject <UBoxComponent>(TEXT("Box Component"));
	BoxComponent->InitBoxExtent(FVector(10.f, 10.f, 10.f));
	SetRootComponent(BoxComponent);


	//TODO
	//BoxComponent->SetCollisionProfileName("OverlapProfile");
	//BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AEnemySpawner::LeftBoxOnOverlapBegin);

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

