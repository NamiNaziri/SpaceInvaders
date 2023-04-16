// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawner.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "EnemyBasePawn.h"

#include "SpaceInvaders/Launcher/ProjectileLauncher.h"
#include "SpaceInvaders/Game/CoreGameState.h"
#include "SpaceInvaders/Game/CoreGameMode.h"

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
	FlushPersistentDebugLines(GetWorld());
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
	//SpawnAllEnemies();
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
	CurrentHeightLevel++;
	SetMovementDirection(EMovementDirection::Down);
	Direction *= -1;
}

void AEnemySpawner::LeftBoxOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == this || OtherActor == GetOwner())
	{
		return;
	}
	CurrentHeightLevel++;


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
			if (CurrentHeightLevel <= MaxHeightLevel)
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
	if (EnemyClass.IsValidIndex(EnemyTypeIndex))
	{
		if (EnemyClass[EnemyTypeIndex])
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;

			// Spawn the actor
			AEnemyBasePawn* SpawnedEnemy = GetWorld()->SpawnActor<AEnemyBasePawn>(EnemyClass[EnemyTypeIndex], SpawnLocation, SpawnRotation, SpawnParams);
			if (SpawnedEnemy)
			{

				Enemies.Push(SpawnedEnemy);
				SpawnedEnemy->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("AEnemySpawner::SpawnAnEnemy: Enemy did not spawned"));
			}
		}

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

	ACoreGameState* GBS = Cast<ACoreGameState>(GetWorld()->GetGameState());
	if (bShouldRegisterToGameState && IsValid(GBS))
	{
		GBS->EnemyDestroyed();
	}
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
	
	if (ActiveShooters[EnemyColumn] == EnemyRow)
	{
		for (int r = EnemyRow - 1; r >= 0; r--)
		{
			if (GetEnemy(r, EnemyColumn)->IsEnemyEnable())
			{
				ActiveShooters[EnemyColumn] = r;
				return;
			}
		}
		// all enmies from "EnemyColumn" are destroyed.
		ActiveShooters.Remove(EnemyColumn);
	}
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
	if (!bEnemiesCanShoot)
	{
		return;
	}

	//TODO change the timer!?
	TArray<int32> KeyArray;
	ActiveShooters.GenerateKeyArray(KeyArray);
	if (ActiveShooters.Num() > 0)
	{
		int RandomIndex = FMath::RandRange(0, KeyArray.Num() - 1);

		int RandomEnemyColumn = KeyArray[RandomIndex];
		int RandomEnemyRow = ActiveShooters[RandomEnemyColumn];
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


}

void AEnemySpawner::InitializeSpawner()
{
	MovementDirection = EMovementDirection::Right;
	Direction = 1.f;
	SetActorLocation(GetActorLocation() - GetActorUpVector() * CurrentHeightLevel * VerticalMovementStride);

	/*
	*	Spawner starts at freeze state. by calling the Delay movement it would wait for DelayDuration and the changes the 
	*	Movement mode to normal. This way, we can achieve a non-timer based movement by setting the DelayInterval to a high numebr.
	*/
	DelayMovmenet();

	GetWorldTimerManager().SetTimer(
		TimerHandle_DelayMovement,
		this,
		&AEnemySpawner::DelayMovmenet,
		DelayInterval + DelayDuration,
		true);

	for (int i = 0; i < Column; i++)
	{
		DestroyedEnemiesPerColumn.Push(0);
		ActiveShooters.Emplace(i, Row - 1);
	}

	/* Adjust edge screen collision boxes */

	InitializeEdgeScreenBoxes();

	/* Spawn Enemies*/
	SpawnAllEnemies();

	for (auto& Enemy : Enemies)
	{
		Enemy->OnEnemyDestroyed.AddDynamic(this, &AEnemySpawner::OnEnemyDestroyed);
	}

	/* Set timer for fire rate*/
	FTimerDelegate TimerDelagate = FTimerDelegate::CreateUObject(this, &AEnemySpawner::FireAtPlayer);
	GetWorldTimerManager().SetTimer(
		TimerHandle_FireAtPlayer,
		TimerDelagate,
		FireRate,
		true);


	RemainingEnemyCount = Row * Column;
	CurrentMovementSpeed = MovementSpeed;

	/* Register enemy count to the game state. */
	ACoreGameState* GBS = Cast<ACoreGameState>(GetWorld()->GetGameState());
	if (bShouldRegisterToGameState && IsValid(GBS))
	{
		GBS->InitRemainingEnemiesCount(RemainingEnemyCount);
	}

}

void AEnemySpawner::InitializeEdgeScreenBoxes()
{
	LeftBoxCurrentColumn = 1;
	RightBoxCurrentColumn = Column;

	FVector StartPoint = -GetActorRightVector() * boxMarginToScreenEnd;
	FVector EndPoint = GetActorRightVector() * (((Column - 1) * HorizontalStride) + boxMarginToScreenEnd);

	LeftBoxComponent->SetRelativeLocation(StartPoint);
	RightBoxComponent->SetRelativeLocation(EndPoint);

	LeftBoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	RightBoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	if (LeftBoxCurrentColumn == RightBoxCurrentColumn)
	{
		LeftBoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AEnemySpawner::SpawnAllEnemies()
{

	FVector StartPoint = -GetActorRightVector() * boxMarginToScreenEnd;
	FVector EndPoint = GetActorRightVector() * (((Column - 1) * HorizontalStride) + boxMarginToScreenEnd);

	FlushPersistentDebugLines(GetWorld());

	//DrawDebugSphere(GetWorld(), GetActorLocation(), 10.f, 12, FColor::Red, true);

	LeftBoxComponent->SetRelativeLocation(StartPoint);
	RightBoxComponent->SetRelativeLocation(EndPoint);
	DestroyAllEnemies();
	for (int i = 0; i < Row; i++)
	{
		int EnemyTypeIndex = 0;
		if (EnemySpawnMode == EEnemySpawnMode::Row_Random)
		{
			EnemyTypeIndex = FMath::RandRange(0, EnemyClass.Num() - 1);
		}
		else if (EnemySpawnMode == EEnemySpawnMode::Row_Sequencial)
		{
			EnemyTypeIndex = FMath::Clamp(i, 0, EnemyClass.Num() - 1);
		}

		for (int j = 0; j < Column; j++)
		{
			if (EnemySpawnMode == EEnemySpawnMode::Individual_Random)
			{
				EnemyTypeIndex = FMath::RandRange(0, EnemyClass.Num() - 1);
			}

			FVector Center = GetActorLocation();
			Center += GetActorRightVector() * j * HorizontalStride;
			Center -= GetActorUpVector() * i * VerticalStride;
			//DrawDebugBox(GetWorld(), Center, FVector(50.f, 50.f, 50.f), FColor::Blue, true);
			SpawnAnEnemy(EnemyTypeIndex, Center, GetActorRotation());
		}
	}
}

void AEnemySpawner::ResetSpawner(int Level)
{
	// Enemies shoot faster in each level
	FireRate *= 0.9;
	FireRate = FMath::Clamp(FireRate, MaxFireRate, FireRate);

	//update the height and reset the location;
	MovementDirection = EMovementDirection::Right;
	Direction = 1.f;
	CurrentHeightLevel = FMath::Clamp(Level, 0, MaxHeightLevel);

	SetActorLocation(GetActorLocation() - GetActorUpVector() * CurrentHeightLevel * VerticalMovementStride);

	DelayMovmenet();

	GetWorldTimerManager().SetTimer(
		TimerHandle_DelayMovement,
		this,
		&AEnemySpawner::DelayMovmenet,
		DelayInterval + DelayDuration,
		true);

	DestroyedEnemiesPerColumn.Empty();
	ActiveShooters.Empty();
	for (int i = 0; i < Column; i++)
	{
		DestroyedEnemiesPerColumn.Push(0);
		ActiveShooters.Emplace(i, Row - 1);
	}

	// reset box colliders
	InitializeEdgeScreenBoxes();


	FTimerDelegate TimerDelagate = FTimerDelegate::CreateUObject(this, &AEnemySpawner::FireAtPlayer);
	GetWorldTimerManager().SetTimer(
		TimerHandle_FireAtPlayer,
		TimerDelagate,
		FireRate,
		true);

	RemainingEnemyCount = Row * Column;
	CurrentMovementSpeed = MovementSpeed;


	ACoreGameState* GBS = Cast<ACoreGameState>(GetWorld()->GetGameState());
	if (bShouldRegisterToGameState && IsValid(GBS))
	{
		GBS->InitRemainingEnemiesCount(RemainingEnemyCount);
	}
	
	for (auto& Enemy : Enemies)
	{
		Enemy->ResetEnemy();
	}
}

void AEnemySpawner::PauseAllTimers()
{
	GetWorldTimerManager().PauseTimer(TimerHandle_DelayMovement);
	GetWorldTimerManager().PauseTimer(TimerHandle_ResetMovement);
	GetWorldTimerManager().PauseTimer(TimerHandle_FireAtPlayer);
}

void AEnemySpawner::SetEdgeScreenCollision(bool bEnable)
{
	if (bEnable)
	{
		LeftBoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		RightBoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		if (LeftBoxCurrentColumn == RightBoxCurrentColumn)
		{
			LeftBoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}
	else
	{
		LeftBoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		RightBoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AEnemySpawner::SetEnemiesCanShoot(bool bCanShoot)
{
	bEnemiesCanShoot = bCanShoot;
}

const TArray<TObjectPtr<AEnemyBasePawn>>& AEnemySpawner::GetEnemies()
{
	return Enemies;
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
