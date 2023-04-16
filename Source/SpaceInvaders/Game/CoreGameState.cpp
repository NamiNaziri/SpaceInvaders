// Fill out your copyright notice in the Description page of Project Settings.


#include "CoreGameState.h"
#include "CoreGameMode.h"
EGameProgressionState ACoreGameState::GetCurrentState()
{
    UpdateGameState();
    return CurrentState;
}

void ACoreGameState::EnemyDestroyed()
{
    this->EnemiesLeft -= 1;
    UE_LOG(LogTemp, Warning, TEXT("Left Enemies: %d"), EnemiesLeft);
    if (EnemiesLeft == 0)
    {
        AdvanceLevel();
    }
}

void ACoreGameState::InitEnemiesLeft(float MaxEnemies)
{
    this->EnemiesLeft = MaxEnemies;
}

void ACoreGameState::AdvanceLevel()
{

    UE_LOG(LogTemp, Warning, TEXT("Level Advanced"));
    ACoreGameMode* GameMode = Cast<ACoreGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
    GameMode->AdvanceLevel();

}

void ACoreGameState::UpdateGameState()
{
}
