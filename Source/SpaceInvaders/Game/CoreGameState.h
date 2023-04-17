// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "CoreGameState.generated.h"

/**
 *  This class oversees the progression of each level, which is a relatively straightforward process. 
 *	The number of enemies remaining in a level serves as a key determining factor for advancement to the next stage.
 */
UCLASS()
class SPACEINVADERS_API ACoreGameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable)
		void EnemyDestroyed();

	UFUNCTION(BlueprintCallable)
		void InitRemainingEnemiesCount(float MaxEnemies);

	UFUNCTION(BlueprintCallable)
		void AdvanceLevel();

protected:

	UPROPERTY()
		int RemainingEnemies = 0;
};
