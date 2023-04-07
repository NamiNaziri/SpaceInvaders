// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBaseActor.h"

#include <Components/BoxComponent.h>

// Sets default values
AEnemyBaseActor::AEnemyBaseActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	BoxComponent = CreateDefaultSubobject <UBoxComponent>(TEXT("Box Component"));
	BoxComponent->InitBoxExtent(FVector(50.f, 50.f, 50.f));
	SetRootComponent(BoxComponent);
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AEnemyBaseActor::OnBoxBeginOverlap);

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

void AEnemyBaseActor::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//TODO make this better
	UE_LOG(LogTemp, Warning, TEXT("AEnemyBaseActor::OnBoxBeginOverlap"))
	this->SetActorHiddenInGame(true);
	this->SetActorEnableCollision(false);
	this->SetActorLocation(FVector(0.f, -1000.f, 0.f));
	OnEnemyHit.Broadcast(this);
}

