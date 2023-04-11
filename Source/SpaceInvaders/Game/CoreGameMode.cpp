// Fill out your copyright notice in the Description page of Project Settings.


#include "CoreGameMode.h"
#include "../Enemy/EnemySpawner.h"

void ACoreGameMode::AdvanceLevel()
{
	this->Level++;
	if (Spawner)
	{
		Spawner->ResetSpawner(Level);
	}
}

void ACoreGameMode::SetSpawner(AEnemySpawner* NewSpawner)
{
	this->Spawner = NewSpawner;
}
