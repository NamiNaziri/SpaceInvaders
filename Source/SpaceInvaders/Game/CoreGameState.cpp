// Fill out your copyright notice in the Description page of Project Settings.


#include "CoreGameState.h"
#include "CoreGameMode.h"

void ACoreGameState::EnemyDestroyed()
{
    this->RemainingEnemies -= 1;
    UE_LOG(LogTemp, Warning, TEXT("Left Enemies: %d"), RemainingEnemies);
    if (RemainingEnemies == 0)
    {
        AdvanceLevel();
    }
}

void ACoreGameState::InitRemainingEnemiesCount(float MaxEnemies)
{
    this->RemainingEnemies = MaxEnemies;
}

void ACoreGameState::AdvanceLevel()
{
    UE_LOG(LogTemp, Warning, TEXT("Level Advanced"));
    ACoreGameMode* GameMode = Cast<ACoreGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
    GameMode->AdvanceLevel();
}
