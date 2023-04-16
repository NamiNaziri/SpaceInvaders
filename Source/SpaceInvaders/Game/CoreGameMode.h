// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CoreGameMode.generated.h"

class AEnemySpawner;
class APlayerBaseController;
class ACoreHUD;
class ASpawnLocationActor;
class AEnemyBasePawn;

UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRemainingTimeUpdated, float, RemainingTime);

UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLevelAdvanced, int, Level);


/*
 * This class oversees the progression of the game.
 */
UCLASS()
class SPACEINVADERS_API ACoreGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:

	ACoreGameMode();

	virtual void StartPlay() override;

	void AdvanceLevel();

	UPROPERTY(BlueprintAssignable)
		FOnRemainingTimeUpdated OnRemainingTimeUpdated;

	UPROPERTY(BlueprintAssignable)
		FOnLevelAdvanced OnLevelAdvanced;

	virtual void PostLogin(APlayerController* NewPlayer);

	UFUNCTION(BlueprintCallable)
		void PlayerDied();

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

	/* New level countdown(seconds) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Level|Flow")
		int NewLevelCountdown = 3.f;


	/*	Spawn new ufo every UFOSpawnInterval seconds.
	*	Note that at the end, each ufo will be spawned after around UFOSpawnInterval + UFOLifeSpan seconds
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Level|UFO")
		float UFOSpawnInterval = 5.f;

	/** allows adding random time to spawn interval time */
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "Level|UFO")
		float UFOSpawnRandomDeviation = 0.f;

	/* Destroy UFO after a duration of UFOLifeSpan seconds has elapsed since it is spawned.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Level|UFO")
		float UFOLifeSpan = 3.f;

	/* allows adding random time to UFO Life Span */
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "Level|UFO")
		float UFOLifeSpanRandomDeviation = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Level|UFO")
		float UFOColisionDisableDelay = 1.f;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Level|Sound Cue")
		TObjectPtr<USoundBase> UFOSoundCue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Level|Sound Cue")
		TObjectPtr<USoundBase> GameOverSoundCue;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Level|Spawn")
		TSubclassOf<AEnemySpawner> UFOSpawnerClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Level|Spawn")
		TSubclassOf<AEnemySpawner> EnemySpawnerClass;


	UPROPERTY()
		TObjectPtr<UAudioComponent> UFOAudioComponent;

	UPROPERTY()
		TObjectPtr<ASpawnLocationActor> SpawnLocationActor;

	UPROPERTY()
		TObjectPtr<AEnemySpawner> UFOSpawner;

	UPROPERTY()
		TObjectPtr<AEnemyBasePawn> UFOPawn;

	
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

	UFUNCTION()
		void OnUFOPawnDestroyed(AEnemyBasePawn* Enemy);

};
