// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CoreGameMode.generated.h"

class AEnemySpawner;
class APlayerBaseController;
class ACoreHUD;
class ASpawnLocationActor;

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


	virtual void StartPlay() override;

	void AdvanceLevel();

	UPROPERTY(BlueprintAssignable)
		FOnRemainingTimeUpdated OnRemainingTimeUpdated;

	virtual void PostLogin(APlayerController* NewPlayer);

protected: 

	UPROPERTY()
		int Level = 1;

	UPROPERTY()
		FTimerHandle TimerHandle_StartNewLevel;

	UPROPERTY()
		FTimerHandle TimerHandle_SpawnUFO;

	UPROPERTY()
		FTimerHandle TimerHandle_DestoryUFO;

	UPROPERTY()
		FTimerHandle TimerHandle_UFOCollisionDisable;

	// New level countdown(seconds)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Level|Flow")
		int NewLevelCountdown = 3.f;

	/* Destroy UFO after a duration of UFOLifeSpan seconds has elapsed since it is spawned.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Level|UFO")
		float UFOLifeSpan = 3.f;

	/** allows adding random time to UFO Life Span */
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "Level|UFO")
		float UFOLifeSpanRandomDeviation = 0.f;
	
	/*	Spawn new ufo every UFOSpawnInterval seconds. 
	*	Note that at the end, each ufo will be spawned after around UFOSpawnInterval + UFOLifeSpan seconds 
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Level|UFO")
		float UFOSpawnInterval = 5.f;

	/** allows adding random time to spawn interval time */
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "Level|UFO")
		float UFOSpawnRandomDeviation = 0.f;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Level|UFO")
		float UFOColisionDisableDelay = 1.f;


	UPROPERTY()
		TObjectPtr<ASpawnLocationActor> SpawnLocationActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Level|Spawn")
		TSubclassOf<AEnemySpawner>   UFOSpawnerClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Level|Spawn")
		TSubclassOf<AEnemySpawner> EnemySpawnerClass;

	UPROPERTY()
		TObjectPtr<AEnemySpawner> UFOSpawner;

	UPROPERTY()
		float RemainingTime;

	

	UPROPERTY()
		TObjectPtr<AEnemySpawner> EnemySpawner;

	UPROPERTY()
		TObjectPtr<APlayerBaseController> PlayerController;

	UPROPERTY()
		TObjectPtr <ACoreHUD> CoreHUD;


	UFUNCTION()
		void StartNewLevel();

	UFUNCTION()
		void SpawnUFO();

	UFUNCTION()
		void DestoryUFO();

	UFUNCTION()
		void OnUFOCollisionDisable();
};
