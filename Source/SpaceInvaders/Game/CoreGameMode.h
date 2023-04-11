// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CoreGameMode.generated.h"

class AEnemySpawner;

/**
 * 
 */
UCLASS()
class SPACEINVADERS_API ACoreGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	void AdvanceLevel();
	void SetSpawner(AEnemySpawner* NewSpawner);

protected: 
	UPROPERTY()
		int Level = 1;


	UPROPERTY()
		TObjectPtr<AEnemySpawner> Spawner;


};
