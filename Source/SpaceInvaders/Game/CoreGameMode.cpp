// Fill out your copyright notice in the Description page of Project Settings.


#include "CoreGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"

#include "SpaceInvaders/Enemy/EnemySpawner.h"
#include "SpaceInvaders/Player/PlayerBaseController.h"
#include "SpaceInvaders/Game/CoreHUD.h"
#include "SpaceInvaders/Game/SpawnLocationActor.h"
#include "SpaceInvaders/Enemy/EnemyBasePawn.h"

ACoreGameMode::ACoreGameMode()
{
}

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
		if (SpawnLocationActor)
		{
			EnemySpawner->SetActorLocation(SpawnLocationActor->EnemySpawnerLocation);
		}
	}
}

void ACoreGameMode::AdvanceLevel()
{
	if (EnemySpawner)
	{
		EnemySpawner->PauseAllTimers();
	}

	// Init timer to tick every second.
	RemainingTime = NewLevelCountdown;
	GetWorldTimerManager().SetTimer(TimerHandle_StartNewLevel, this, &ACoreGameMode::StartNewLevel, 1.f, true);
}

void ACoreGameMode::StartNewLevel()
{
	RemainingTime--;
	if (RemainingTime == 0)
	{
		this->Level++;

		GetWorldTimerManager().ClearTimer(TimerHandle_StartNewLevel);
		OnLevelAdvanced.Broadcast(Level);
		
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
		
		if (UFOSpawner)
		{
			const TArray<TObjectPtr<AEnemyBasePawn>> UFOEnemyList = UFOSpawner->GetEnemies();
			if (UFOEnemyList.IsValidIndex(0))
			{
				// bind an event so we know when the ufo is destroyed by the player.
				UFOEnemyList[0]->OnEnemyDestroyed.AddDynamic(this, &ACoreGameMode::OnUFOPawnDestroyed);
			}
		}
		
		UFOAudioComponent = UGameplayStatics::SpawnSound2D(GetWorld(), UFOSoundCue);
	}
}

void ACoreGameMode::DestoryUFO()
{
	if (UFOSpawner)
	{
		GetWorldTimerManager().SetTimer(TimerHandle_SpawnUFO, this, &ACoreGameMode::SpawnUFO, UFOSpawnInterval, false);
		UFOSpawner->SetEdgeScreenCollision(false);
		UFOSpawner->SetLifeSpan(7.f);
		if (UFOAudioComponent)
		{
			UFOAudioComponent->StopDelayed(1.f);
		}
	}
}

void ACoreGameMode::OnUFOCollisionDisable()
{
	UFOSpawner->SetEdgeScreenCollision(true);
}

void ACoreGameMode::OnUFOPawnDestroyed(AEnemyBasePawn* Enemy)
{
	if (UFOAudioComponent)
	{
		UFOAudioComponent->Stop();
	}
}

void ACoreGameMode::PlayerDied()
{
	if (UFOAudioComponent)
	{
		UFOAudioComponent->Stop();
	}

	UGameplayStatics::PlaySound2D(GetWorld(), GameOverSoundCue);
}


void ACoreGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	this->PlayerController = Cast<APlayerBaseController>(NewPlayer);
	this->CoreHUD = Cast <ACoreHUD>(NewPlayer->GetHUD());
}
