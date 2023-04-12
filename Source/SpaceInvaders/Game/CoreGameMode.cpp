// Fill out your copyright notice in the Description page of Project Settings.


#include "CoreGameMode.h"
#include "SpaceInvaders/Enemy/EnemySpawner.h"
#include "Kismet/GameplayStatics.h"
#include "SpaceInvaders/Player/PlayerBaseController.h"
#include "SpaceInvaders/Game/CoreHUD.h"

void ACoreGameMode::AdvanceLevel()
{
	this->Level++;
	
	if (Spawner)
	{
		Spawner->PauseAllTimers();
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
		
		if (Spawner)
		{
			Spawner->ResetSpawner(Level);
		}
	}
}

void ACoreGameMode::SetSpawner(AEnemySpawner* NewSpawner)
{
	this->Spawner = NewSpawner;
}

void ACoreGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	this->PlayerController = Cast<APlayerBaseController>(NewPlayer);
	this->CoreHUD = Cast <ACoreHUD>(NewPlayer->GetHUD());
}
