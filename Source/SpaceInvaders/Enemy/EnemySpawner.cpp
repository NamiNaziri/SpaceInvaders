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
	
	SetActorLocation(GetActorLocation() - GetActorUpVector() * currentHeightLevel * VerticalMovementStride);

	GetWorldTimerManager().SetTimer(TimerHandle_DelayMovement, this, &AEnemySpawner::DelayMovmenet, DelayInterval + DelayDuration, true);

}

// Called every frame
void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Move(DeltaTime);


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
		int EnemyTypeIndex = FMath::RandRange(0, EnemyType.Num() - 1);
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

void AEnemySpawner::SetMovementDirection(TEnumAsByte<EMovementDirection> NewMovementDirection)
{
	this->MovementDirection = NewMovementDirection;
}

void AEnemySpawner::SetMovementMode(TEnumAsByte<EEnemyMovementMode> NewMovementMode)
{
	this->MovementMode = NewMovementMode;
}

void AEnemySpawner::RightBoxOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	currentHeightLevel++;
	SetMovementDirection(EMovementDirection::Down);
	bRightBoxOverlapped = true;
}

void AEnemySpawner::LeftBoxOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	currentHeightLevel++;
	SetMovementDirection(EMovementDirection::Down);
	bRightBoxOverlapped = false;
}

void AEnemySpawner::Move(float DeltaTime)
{

	if (MovementMode == EEnemyMovementMode::Freezed)
	{
		return;
	}


	FVector CurrentLocation = GetActorLocation();

	switch (MovementDirection)
	{
		case EMovementDirection::Right:
			
			SetActorLocation(CurrentLocation + GetActorRightVector() * MovementSpeed * DeltaTime);
			break;

		case EMovementDirection::Left:
			SetActorLocation(CurrentLocation - GetActorRightVector() * MovementSpeed * DeltaTime);
			break;

		case EMovementDirection::Down:
			if (currentHeightLevel <= maxHeightLevel)
			{
				SetActorLocation(CurrentLocation - GetActorUpVector() * VerticalMovementStride);

				/*Delaying the movement of enemies when they reach the end*/

				//reset the timer that delays the movment
				GetWorldTimerManager().SetTimer(TimerHandle_DelayMovement, this, &AEnemySpawner::DelayMovmenet, DelayInterval + DelayDuration, true);

				// Manually delay the movement
				DelayMovmenet();
			}
			
			if (bRightBoxOverlapped)
			{
				SetMovementDirection(EMovementDirection::Left);
			}
			else 
			{
				SetMovementDirection(EMovementDirection::Right);
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

void AEnemySpawner::DelayMovmenet()
{
	FTimerDelegate TimerDelagate = FTimerDelegate::CreateUObject(this, &AEnemySpawner::ResetMovement);
	GetWorldTimerManager().SetTimer(
		TimerHandle_ResetMovement,
		TimerDelagate,
		DelayDuration,
		false);

	SetMovementMode(EEnemyMovementMode::Freezed);
}

void AEnemySpawner::ResetMovement()
{
	
	SetMovementMode(EEnemyMovementMode::Normal);
	GetWorldTimerManager().ClearTimer(TimerHandle_ResetMovement);
}