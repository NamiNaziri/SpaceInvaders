// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawner.h"
#include <Components/BoxComponent.h>
#include "GameFramework/Actor.h"
#include "EnemyBasePawn.h"
#include "../Launcher/ProjectileLauncher.h"
#include "../Game/CoreGameState.h"
#include "../Game/CoreGameMode.h"

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
	LeftBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AEnemySpawner::LeftBoxOnOverlapBegin);


	RightBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Right Box Component"));
	RightBoxComponent->SetupAttachment(RootComponent);
	RightBoxComponent->InitBoxExtent(FVector(50.f, 50.f, 50.f));
	RightBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AEnemySpawner::RightBoxOnOverlapBegin);
	
}

// Called when the game starts or when spawned
void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();

	InitializeSpawner();
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
	if (OtherActor == this || OtherActor == GetOwner())
	{
		return;
	}
	currentHeightLevel++;
	SetMovementDirection(EMovementDirection::Down);
	Direction *= -1;
}

void AEnemySpawner::LeftBoxOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == this || OtherActor == GetOwner())
	{
		return;
	}
	currentHeightLevel++;
	SetMovementDirection(EMovementDirection::Down);
	Direction *= -1;
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
			
			SetActorLocation(CurrentLocation + GetActorRightVector() * CurrentMovementSpeed * DeltaTime);
			break;

		case EMovementDirection::Left:
			SetActorLocation(CurrentLocation - GetActorRightVector() * CurrentMovementSpeed * DeltaTime);
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
			
			if (Direction < 0.f)
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
		AEnemyBasePawn* SpawnedEnemy = GetWorld()->SpawnActor<AEnemyBasePawn>(EnemyType[EnemyTypeIndex], SpawnLocation, SpawnRotation, SpawnParams);
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

void AEnemySpawner::OnEnemyDestroyed(AEnemyBasePawn* Enemy)
{
	RemainingEnemyCount--;
	int Index = Enemies.Find(Enemy);
	UpdateEdgeScreenBoxes(Index);
	UpdateShooter(Index);
	UpdateMovementSpeed();
}

void AEnemySpawner::UpdateEdgeScreenBoxes(int EnemyIndex)
{
	//int EnemyRow = (index + 1) / Column;
	int EnemyColumn = (EnemyIndex % Column) + 1; // starting from 1

	DestroyedEnemiesPerColumn[EnemyColumn - 1]++;

	if (DestroyedEnemiesPerColumn[EnemyColumn - 1] == Row)
	{
		if (LeftBoxCurrentColumn == EnemyColumn)
		{
			LeftBoxCurrentColumn++;
			for (int i = EnemyColumn + 1; i <= Column; i++)
			{
				if (DestroyedEnemiesPerColumn[i - 1] == Row)
				{
					LeftBoxCurrentColumn++;
				}
				else
				{
					break;
				}
			}
		}


		if (RightBoxCurrentColumn == EnemyColumn)
		{
			RightBoxCurrentColumn--;
			for (int i = EnemyColumn - 1; i > 0; i--)
			{
				if (DestroyedEnemiesPerColumn[i - 1] == Row)
				{
					RightBoxCurrentColumn--;
				}
				else
				{
					break;
				}
			}
		}
	}

	if (LeftBoxCurrentColumn == RightBoxCurrentColumn)
	{
		LeftBoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	FVector LeftBoxNewPos = GetActorRightVector() * (((LeftBoxCurrentColumn - 1) * HorizontalStride) - boxMarginToScreenEnd);
	FVector RightBoxNewPos = GetActorRightVector() * (((RightBoxCurrentColumn - 1) * HorizontalStride) - boxMarginToScreenEnd);


	LeftBoxComponent->SetRelativeLocation(LeftBoxNewPos);
	RightBoxComponent->SetRelativeLocation(RightBoxNewPos);
}

void AEnemySpawner::UpdateShooter(int EnemyIndex)
{
	int EnemyRow = (EnemyIndex / Column); // starting from 0
	int EnemyColumn = (EnemyIndex % Column); // starting from 0

	if (ShootersIndex[EnemyColumn] == EnemyRow)
	{
		for (int r = EnemyRow - 1; r >= 0; r--)
		{
			if (GetEnemy(r, EnemyColumn)->IsEnemyEnable())
			{
				ShootersIndex[EnemyColumn] = r;
				return;
			}
		}
	}

	ShootersIndex[EnemyColumn] = -1;

}

TObjectPtr<AEnemyBasePawn> AEnemySpawner::GetEnemy(int r, int c)
{
	if (Enemies.IsValidIndex(r * Column + c))
	{
		return Enemies[r * Column + c];
	}
	return nullptr;
}

void AEnemySpawner::FireAtPlayer()
{
	//TODO change the timer!?

	int RandomEnemyColumn = FMath::RandRange(0, Column - 1);
	int RandomEnemyRow = ShootersIndex[RandomEnemyColumn];
	TObjectPtr<AEnemyBasePawn> enemy = GetEnemy(RandomEnemyRow, RandomEnemyColumn);
	if (enemy)
	{
		enemy->Shoot();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("AEnemySpawner::FireAtPlayer -> index is not valid"));
	}
	

}

void AEnemySpawner::InitializeSpawner()
{
	SetActorLocation(GetActorLocation() - GetActorUpVector() * currentHeightLevel * VerticalMovementStride);

	GetWorldTimerManager().SetTimer(
		TimerHandle_DelayMovement,
		this,
		&AEnemySpawner::DelayMovmenet,
		DelayInterval + DelayDuration,
		true);

	for (int i = 0; i < Column; i++)
	{
		DestroyedEnemiesPerColumn.Push(0);
		ShootersIndex.Push(Row - 1);
	}

	LeftBoxCurrentColumn = 1;
	RightBoxCurrentColumn = Column;

	for (auto& Enemy : Enemies)
	{
		Enemy->OnEnemyDestroyed.AddDynamic(this, &AEnemySpawner::OnEnemyDestroyed);
	}


	FTimerDelegate TimerDelagate = FTimerDelegate::CreateUObject(this, &AEnemySpawner::FireAtPlayer);
	GetWorldTimerManager().SetTimer(
		TimerHandle_FireAtPlayer,
		TimerDelagate,
		FireRate,
		true);

	RemainingEnemyCount = Row * Column;
	CurrentMovementSpeed = MovementSpeed;

	ACoreGameState* GBS = Cast<ACoreGameState>(GetWorld()->GetGameState());
	if (IsValid(GBS))
	{
		GBS->InitEnemiesLeft(RemainingEnemyCount);
	}

	ACoreGameMode* GameMode = Cast<ACoreGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (IsValid(GameMode))
	{
		GameMode->SetSpawner(this);
	}

}

void AEnemySpawner::ResetSpawner(int Level)
{

	//update the height and reset the location;
	MovementDirection = EMovementDirection::Right;
	currentHeightLevel = FMath::Clamp(Level, 0, maxHeightLevel);
	SetActorLocation(FVector(150.f, 100.f, 1260.f));


	SetActorLocation(GetActorLocation() - GetActorUpVector() * currentHeightLevel * VerticalMovementStride);

	GetWorldTimerManager().SetTimer(
		TimerHandle_DelayMovement,
		this,
		&AEnemySpawner::DelayMovmenet,
		DelayInterval + DelayDuration,
		true);

	DestroyedEnemiesPerColumn.Empty();
	ShootersIndex.Empty();
	for (int i = 0; i < Column; i++)
	{
		DestroyedEnemiesPerColumn.Push(0);
		ShootersIndex.Push(Row - 1);
	}

	// reset box colliders
	LeftBoxCurrentColumn = 1;
	RightBoxCurrentColumn = Column;

	FVector StartPoint = -GetActorRightVector() * boxMarginToScreenEnd;
	FVector EndPoint = GetActorRightVector() * (((Column - 1) * HorizontalStride) + boxMarginToScreenEnd);

	LeftBoxComponent->SetRelativeLocation(StartPoint);
	RightBoxComponent->SetRelativeLocation(EndPoint);

	LeftBoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	RightBoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);


	FTimerDelegate TimerDelagate = FTimerDelegate::CreateUObject(this, &AEnemySpawner::FireAtPlayer);
	GetWorldTimerManager().SetTimer(
		TimerHandle_FireAtPlayer,
		TimerDelagate,
		FireRate,
		true);

	RemainingEnemyCount = Row * Column;
	CurrentMovementSpeed = MovementSpeed;

	ACoreGameState* GBS = Cast<ACoreGameState>(GetWorld()->GetGameState());
	GBS->InitEnemiesLeft(RemainingEnemyCount);


	for (auto& Enemy : Enemies)
	{
		Enemy->Reset();
	}

}

void AEnemySpawner::UpdateMovementSpeed()
{
	float alpha = 1.f - ((float)(RemainingEnemyCount - 1) / (float)((Row * Column) - 1)); // linear blend

	if (SpeedChangeRateCurve)
	{
		alpha = SpeedChangeRateCurve->GetFloatValue(alpha);
	}

	CurrentMovementSpeed = FMath::Lerp(MovementSpeed, MaxMovementSpeed, alpha);
}
