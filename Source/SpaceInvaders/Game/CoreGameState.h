// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "CoreGameState.generated.h"

UENUM(BlueprintType)
enum EGameProgressionState
{
	NotStarted UMETA(DisplayName = "Right"),
	InProgress UMETA(DisplayName = "Left"),
	Finished UMETA(DisplayName = "Down")
};


/**
 * 
 */
UCLASS()
class SPACEINVADERS_API ACoreGameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	EGameProgressionState GetCurrentState();

	void EnemyDestroyed();
	void InitEnemiesLeft(float MaxEnemies);
	void AdvanceLevel();


protected:


	UPROPERTY()
		int EnemiesLeft = 0;

	TEnumAsByte<EGameProgressionState> CurrentState;

	void UpdateGameState();
};
