// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBaseActor.h"

// Sets default values
AEnemyBaseActor::AEnemyBaseActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	SetRootComponent(RootComponent);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AEnemyBaseActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemyBaseActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

