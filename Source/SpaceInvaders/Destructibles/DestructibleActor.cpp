// Fill out your copyright notice in the Description page of Project Settings.


#include "DestructibleActor.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "Delegates/Delegate.h"
#include "Field/FieldSystemActor.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

#include "SpaceInvaders/GameComponents/Health/HealthComponent.h"

// Sets default values
ADestructibleActor::ADestructibleActor()
{

	// The boxcomponent size should only be adjusted using box extent variable
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Scene Component"));

	BoxComponent = CreateDefaultSubobject <UBoxComponent>(TEXT("Box Component"));
	BoxComponent->InitBoxExtent(FVector(152.f, 152.f, 152.f));
	BoxComponent->SetCollisionProfileName(TEXT("OverlapAll"));

	BoxComponent->SetupAttachment(RootComponent);

	PrimaryActorTick.bCanEverTick = true;
	GeometryCollection = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("Geometry Collection"));

	GeometryCollection->SetupAttachment(RootComponent);
	

	HealthComponent = CreateDefaultSubobject <UHealthComponent>(TEXT("Health Component"));
	HealthComponent->OnHealthBecomeZero.AddDynamic(this, &ADestructibleActor::HealthBecomeZero);

	this->OnTakePointDamage.AddDynamic(this, &ADestructibleActor::TakePointDamage);
}


// Called when the game starts or when spawned
void ADestructibleActor::BeginPlay()
{
	Super::BeginPlay();
	
	// How this works?

	// The Actor destroys when it's health reaches zero.
	// Since the anchor is set to the middle of the boxcomponent( or the object itself) we should 
	// move the box so it would reach middle of the object when the health reaches zero (or close to zero with some error)

	float BoxHalfHeight = BoxComponent->GetUnscaledBoxExtent().Z / 2.f;

	BoxAdjustmentOffset = BoxHalfHeight / HealthComponent->GetMaxHealth();

}

// Called every frame
void ADestructibleActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADestructibleActor::OnConstruction(const FTransform& Transform)
{
	if (AnchorField)
	{
		AnchorField->SetActorLocation(this->GetActorLocation());
		AnchorField->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
		GeometryCollection->InitializationFields.Empty();
		GeometryCollection->InitializationFields.Push(AnchorField);
	}
}

float ADestructibleActor::GetRemainingHealth()
{
	return HealthComponent->GetMaxHealth() - HealthComponent->GetCurrentHealth();
}

void ADestructibleActor::TakePointDamage(AActor* DamagedActor, float Damage, AController* InstigatedBy, FVector HitLocation, UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const UDamageType* DamageType, AActor* DamageCauser)
{

	if (DestructionParticleSystem)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DestructionParticleSystem, HitLocation,GetActorRotation());
	}

	if (DebrisFallingSoundCue)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), DebrisFallingSoundCue);
	}

	FVector NewBoxExtent = BoxComponent->GetUnscaledBoxExtent();
	NewBoxExtent.Z -= BoxAdjustmentOffset;
	BoxComponent->SetBoxExtent(NewBoxExtent);

	// 1 means it is been shot from bottom of the object
	float ImpactDirection = HitLocation.Z < GetActorLocation().Z ? 1.f : -1.f;
	BoxComponent->AddRelativeLocation(ImpactDirection * GetActorUpVector() * (BoxAdjustmentOffset));

	HealthComponent->DecreaseHealth(Damage);

	UE_LOG(LogTemp, Warning, TEXT("DestructibleActor Health: %f"), HealthComponent->GetCurrentHealth());
}

void ADestructibleActor::HealthBecomeZero(AActor* OwnerActor)
{
	GeometryCollection->RemoveAllAnchors();

	if (SelfDestructionDamageClass)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		
		// Spawn the actor
		AFieldSystemActor* FieldSystemActor = GetWorld()->SpawnActor<AFieldSystemActor>(SelfDestructionDamageClass, GetActorLocation(), GetActorRotation(), SpawnParams);
		FieldSystemActor->SetLifeSpan(1.f);
	}

	BoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	/* Destroy the object after 4 seconds. 4 seconds is used to be sure that the object is out of the screen.*/
	SetLifeSpan(4.f);
}

