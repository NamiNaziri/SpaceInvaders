// Fill out your copyright notice in the Description page of Project Settings.


#include "CoreGameMode.h"
#include "SpaceInvaders/Enemy/EnemySpawner.h"
#include "Kismet/GameplayStatics.h"
#include "SpaceInvaders/Player/PlayerBaseController.h"
#include "SpaceInvaders/Game/CoreHUD.h"
#include "SpaceInvaders/Game/SpawnLocationActor.h"

void ACoreGameMode::StartPlay()
{
	Super::StartPlay();


	SpawnLocationActor = Cast<ASpawnLocationActor>(UGameplayStatics::GetActorOfClass(GetWorld(), ASpawnLocationActor::StaticClass()));

	if (!SpawnLocationActor)
	{
		UE_LOG(LogTemp, Error, TEXT("No Spawn Location actor found in the level"));
	}

	if (UFOSpawnerClass)
	{
		const float Interval = FMath::FRandRange(FMath::Max(0.0f, UFOSpawnInterval - UFOSpawnRandomDeviation), (UFOSpawnInterval + UFOSpawnRandomDeviation));
		GetWorldTimerManager().SetTimer(TimerHandle_SpawnUFO, this, &ACoreGameMode::SpawnUFO, Interval,false);
		
	}

	if (EnemySpawnerClass )
	{
		FActorSpawnParameters ActorSpawnParameters;
		ActorSpawnParameters.Owner = this;

		EnemySpawner = GetWorld()->SpawnActor<AEnemySpawner>(EnemySpawnerClass, ActorSpawnParameters);
		//EnemySpawner->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
		if (SpawnLocationActor)
		{
			EnemySpawner->SetActorLocation(SpawnLocationActor->EnemySpawnerLocation);
		}
	}
}

void ACoreGameMode::AdvanceLevel()
{
	this->Level++;
	
	if (EnemySpawner)
	{
		EnemySpawner->PauseAllTimers();
	}

	// set timer to tick every second.
	RemainingTime = NewLevelCountdown;
	GetWorldTimerManager().SetTimer(TimerHandle_StartNewLevel, this, &ACoreGameMode::StartNewLevel, 1.f, true);
}

void ACoreGameMode::StartNewLevel()
{
	RemainingTime--;
	if (RemainingTime == 0)
	{
		GetWorldTimerManager().ClearTimer(TimerHandle_StartNewLevel);
		
		if (EnemySpawner)
		{
			if (SpawnLocationActor)
			{
				EnemySpawner->SetActorLocation(SpawnLocationActor->EnemySpawnerLocation);
			}
			EnemySpawner->ResetSpawner(Level);
		}
	}
}

void ACoreGameMode::SpawnUFO()
{
	if (UFOSpawnerClass)
	{

		FActorSpawnParameters ActorSpawnParameters;
		ActorSpawnParameters.Owner = this;

		UFOSpawner = GetWorld()->SpawnActor<AEnemySpawner>(UFOSpawnerClass, ActorSpawnParameters);
		UFOSpawner->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
		if (SpawnLocationActor)
		{
			UFOSpawner->SetActorLocation(SpawnLocationActor->UFOSpawnerLocation);
		}
		UFOSpawner->SetEdgeScreenCollision(false);
		UFOSpawner->SetEnemiesCanShoot(false);

		const float LifeSpan = FMath::FRandRange(FMath::Max(0.0f, UFOLifeSpan - UFOLifeSpanRandomDeviation), (UFOLifeSpan + UFOLifeSpanRandomDeviation));

		GetWorldTimerManager().SetTimer(TimerHandle_DestoryUFO, this, &ACoreGameMode::DestoryUFO, UFOLifeSpan, false);
		GetWorldTimerManager().SetTimer(TimerHandle_UFOCollisionDisable, this, &ACoreGameMode::OnUFOCollisionDisable, UFOColisionDisableDelay, false);
		
		// todo set other spawner related stuff;
	}
}

void ACoreGameMode::DestoryUFO()
{
	if (UFOSpawner)
	{
		GetWorldTimerManager().SetTimer(TimerHandle_SpawnUFO, this, &ACoreGameMode::SpawnUFO, UFOSpawnInterval, false);
		UFOSpawner->SetEdgeScreenCollision(false);
		UFOSpawner->SetLifeSpan(3.f);
	}
}

void ACoreGameMode::OnUFOCollisionDisable()
{
	UFOSpawner->SetEdgeScreenCollision(true);
}


void ACoreGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	this->PlayerController = Cast<APlayerBaseController>(NewPlayer);
	this->CoreHUD = Cast <ACoreHUD>(NewPlayer->GetHUD());
}
