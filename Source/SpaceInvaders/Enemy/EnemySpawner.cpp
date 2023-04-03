// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawner.h"
#include <Components/BoxComponent.h>
#include "GameFramework/Actor.h"
#include "EnemyBaseActor.h"


// Sets default values
AEnemySpawner::AEnemySpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	SetRootComponent(RootComponent);

	LeftBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Left Box Component"));
	LeftBoxComponent->SetupAttachment(RootComponent);
	LeftBoxComponent->InitBoxExtent(FVector(50.f, 50.f, 50.f));
	LeftBoxComponent->SetCollisionProfileName("OverlapProfile");
	LeftBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AEnemySpawner::LeftBoxOnOverlapBegin);


	RightBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Right Box Component"));
	RightBoxComponent->SetupAttachment(RootComponent);
	RightBoxComponent->InitBoxExtent(FVector(50.f, 50.f, 50.f));
	RightBoxComponent->SetCollisionProfileName("OverlapProfile");
	RightBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AEnemySpawner::RightBoxOnOverlapBegin);
	
}

// Called when the game starts or when spawned
void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();

	FTimerDelegate TimerDelegate; 
	TimerDelegate.BindUFunction(this, "Move");

	GetWorldTimerManager().SetTimer(
		MovementTimerHandle,
		TimerDelegate,
		BaseMovennetSpeedSecond / MovennetSpeedMultiplier,
		true,
		-1.f);

	
	SetActorLocation(GetActorLocation() - GetActorUpVector() * currentHeightLevel * VerticalMovementStride);
	//currentHeightLevel = 0;

}

// Called every frame
void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/*FVector StartPoint = -GetActorRightVector() * boxMarginToScreenEnd;
	FVector EndPoint = GetActorRightVector() * (((Column - 1) * HorizontalStride) + boxMarginToScreenEnd);
	FlushPersistentDebugLines(GetWorld());

	DrawDebugSphere(GetWorld(), GetActorLocation(), 10.f, 12, FColor::Red, true);

	LeftBoxComponent->SetRelativeLocation(StartPoint);
	RightBoxComponent->SetRelativeLocation(EndPoint);

	for (int i = 0; i < Row; i++)
	{
		for (int j = 0; j < Column; j++)
		{
			FVector Center = GetActorLocation();
			Center += GetActorRightVector() * j * HorizontalStride;
			Center -= GetActorUpVector() * i * VerticalStride;
			DrawDebugBox(GetWorld(), Center, FVector(50.f, 50.f, 50.f), FColor::Blue, true);

		}
	}*/
}

void AEnemySpawner::OnConstruction(const FTransform& Transform)
{
	FVector StartPoint = - GetActorRightVector() * boxMarginToScreenEnd;
	FVector EndPoint = GetActorRightVector() * (((Column - 1) * HorizontalStride) + boxMarginToScreenEnd);
	//FVector LeftBoxExtent;

	FlushPersistentDebugLines(GetWorld());

	DrawDebugSphere(GetWorld(), GetActorLocation(), 10.f, 12, FColor::Red, true);

	LeftBoxComponent->SetRelativeLocation(StartPoint);
	RightBoxComponent->SetRelativeLocation(EndPoint);
	DestroyAllEnemies();
	for (int i = 0; i < Row; i++)
	{
		int EnemyTypeIndex = FMath::RandRange(0, EnemyType.Num());
		for (int j = 0; j < Column; j++)
		{
			FVector Center = GetActorLocation();
			Center += GetActorRightVector() * j * HorizontalStride;
			Center -= GetActorUpVector() * i * VerticalStride;
			DrawDebugBox(GetWorld(), Center, FVector(50.f, 50.f, 50.f), FColor::Blue, true);
			SpawnAnEnemy(EnemyTypeIndex, Center, GetActorRotation());
		}
	}

}

void AEnemySpawner::RightBoxOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	currentHeightLevel++;
	MovementDirection = EMovementDirection::Down;
	bRightBoxOverlapped = true;
}

void AEnemySpawner::LeftBoxOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	currentHeightLevel++;
	MovementDirection = EMovementDirection::Down;
	bRightBoxOverlapped = false;
}

void AEnemySpawner::Move()
{
	FVector CurrentLocation = GetActorLocation();

	switch (MovementDirection)
	{
		case EMovementDirection::Right:
			SetActorLocation(CurrentLocation + GetActorRightVector() * HorizontalMovementStride);
			break;

		case EMovementDirection::Left:
			SetActorLocation(CurrentLocation - GetActorRightVector() * HorizontalMovementStride);
			break;

		case EMovementDirection::Down:
			if (currentHeightLevel <= maxHeightLevel)
			{
				SetActorLocation(CurrentLocation - GetActorUpVector() * VerticalMovementStride);
			}
			
			if (bRightBoxOverlapped)
			{
				MovementDirection = EMovementDirection::Left;
			}
			else 
			{
				MovementDirection = EMovementDirection::Right;
			}
			break;

		default:
			break;
	}
}

void AEnemySpawner::SpawnAnEnemy(int EnemyTypeIndex, FVector SpawnLocation, FRotator SpawnRotation)
{
	if (EnemyType.IsValidIndex(EnemyTypeIndex))
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		
		// Spawn the actor
		AEnemyBaseActor* SpawnedEnemy = GetWorld()->SpawnActor<AEnemyBaseActor>(EnemyType[EnemyTypeIndex], SpawnLocation, SpawnRotation, SpawnParams);
		Enemies.Push(SpawnedEnemy);
		//FAttachmentTransformRules AttachmentTransformRules;
		SpawnedEnemy->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
	}
	
}

void AEnemySpawner::DestroyAllEnemies()
{
	for (auto enemy : Enemies)
	{
		if(enemy)
			enemy->Destroy();
	}
	Enemies.Empty();
}

