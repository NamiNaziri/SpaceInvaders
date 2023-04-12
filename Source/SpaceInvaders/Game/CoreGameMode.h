// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CoreGameMode.generated.h"

class AEnemySpawner;
class APlayerBaseController;
class ACoreHUD;

UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRemainingTimeUpdated, float, RemainingTime);


/**
 * 
 */
UCLASS()
class SPACEINVADERS_API ACoreGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	void AdvanceLevel();
	void StartNewLevel();
	void SetSpawner(AEnemySpawner* NewSpawner);

	UPROPERTY(BlueprintAssignable)
		FOnRemainingTimeUpdated OnRemainingTimeUpdated;

	virtual void PostLogin(APlayerController* NewPlayer);

protected: 
	UPROPERTY()
		int Level = 1;

	UPROPERTY()
		FTimerHandle TimerHandle_StartNewLevel;

	// New level countdown(seconds)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Level|Flow")
		int NewLevelCountdown = 3;

	UPROPERTY()
		float RemainingTime;

	

	UPROPERTY()
		TObjectPtr<AEnemySpawner> Spawner;

	UPROPERTY()
		TObjectPtr<APlayerBaseController> PlayerController;

	UPROPERTY()
		TObjectPtr <ACoreHUD> CoreHUD;
};
