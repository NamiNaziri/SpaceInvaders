// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Curves/CurveFloat.h"
#include "EnemySpawner.generated.h"

class UBoxComponent;
class AEnemyBasePawn;

UENUM(BlueprintType)		
enum EMovementDirection	
{
	Right UMETA(DisplayName = "Right"),	
	Left UMETA(DisplayName = "Left"),
	Down UMETA(DisplayName = "Down")
};

UENUM(BlueprintType)
enum EEnemyMovementMode
{
	Normal UMETA(DisplayName = "Normal"),
	Freezed UMETA(DisplayName = "Freeze")
};

UENUM(BlueprintType)
enum EEnemySpawnMode
{
	Row_Random UMETA(DisplayName = "Row_Random"), // Chooses a class for each row randomly. (If you want the randomness be unform, you should only have a single item for each class in EnemyClass array. by having several item it's give that class more chance to be chosen)
	Individual_Random UMETA(DisplayName = "Individual_Random"), // Chooses a class for each place randomly. 
	Row_Sequencial UMETA(DisplayName = "Row_Sequencial"), // for row = i it chooses the EnemyClass[i]. So for spawner with 5 row, you should add 5 item to EnemyClass array.
};

/*
*	The class is responsible for spawning enemies in a grid. 
*	The actor's location is the location of the top left corner of this grid.	
*/

UCLASS()
class SPACEINVADERS_API AEnemySpawner : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	AEnemySpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/* This can be used to create the enemies in editor mode, but currently it is disabled*/
	virtual void OnConstruction(const FTransform& Transform) override;

	/* Resets the spawner to it's original state and does the necessary adjustment for new level. I am not using built-in reset function on purpose.*/
	void ResetSpawner(int Level);

	/* Pauses all the timers. Used when advancing levels.*/
	void PauseAllTimers();

	void SetEdgeScreenCollision(bool bEnable);

	void SetEnemiesCanShoot(bool bCanShoot);

	const TArray<TObjectPtr<AEnemyBasePawn>>& GetEnemies();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), category = "Spawner|Structure")
		int32 Row = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), category = "Spawner|Structure")
		int32 Column= 2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), category = "Spawner|Structure")
		float HorizontalStride = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), category = "Spawner|Structure")
		float VerticalStride = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), category = "Spawner|Structure")
		float boxMarginToScreenEnd = 1.f;

	/* 
	*	Should this spawner register itself to the game state in order to affect level advancement or not. 
	*	 e.g. The UFO sets this to false since destroying it doesn't affect the level advancement.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), category = "Spawner|Structure")
		bool bShouldRegisterToGameState = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), category = "Spawner|Structure")
		TEnumAsByte<EEnemySpawnMode> EnemySpawnMode = EEnemySpawnMode::Row_Random;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), category = "Spawner|Enemy")
		TArray<TSubclassOf<AEnemyBasePawn>> EnemyClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), category = "Spawner|Enemy")
		TArray<TObjectPtr<AEnemyBasePawn>> Enemies;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), category = "Spawner|Enemy")
		bool bEnemiesCanShoot = true;

	/* 
	*	Row and Column of active an available shooters.
	*	<Key=Column, Value=Row> 
	*/
	TMap<int32, int32> ActiveShooters;
	
	/* Number of enemies destroyed per column. Used for when we want to update the edge screen colliders. */
	TArray<int32> DestroyedEnemiesPerColumn;

	int32 LeftBoxCurrentColumn;
	int32 RightBoxCurrentColumn;

	int RemainingEnemyCount;

	/* Base movement speed */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), category = "Spawner|Movement")
		float MovementSpeed = 400.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), category = "Spawner|Movement")
	/* Current Movement Speed */
	float CurrentMovementSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), category = "Spawner|Movement")
		float MaxMovementSpeed = 1000.f;
	/*
	* This curve needs to be normalized between zero and one, as it will determine how the speed increases when an enemy is destroyed.
	* If the curve does not exist, a linear rate will be used instead.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), category = "Spawner|Movement")
		UCurveFloat* SpeedChangeRateCurve;

	/* Wait (stop) after every DelayInterval(seconds) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), category = "Spawner|Movement")
		float DelayInterval = 1.f;

	/* After the delay interval is reached, wait for DelayDuration(seconds) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), category = "Spawner|Movement")
		float DelayDuration = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), category = "Spawner|Movement")
		float VerticalMovementStride = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), category = "Spawner|Movement")
		int32 MaxHeightLevel = 10;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), category = "Spawner|Movement")
		int32 CurrentHeightLevel = 0;

	
	// Rate at which a weapon can fire projectiles. 1 shot per every FireRate second.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Spawner|Enemy")
		float FireRate = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Spawner|Enemy")
		float MaxFireRate = 0.5f;

	FTimerHandle TimerHandle_DelayMovement;

	FTimerHandle TimerHandle_ResetMovement;

	FTimerHandle TimerHandle_FireAtPlayer;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		TObjectPtr<UBoxComponent> LeftBoxComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		TObjectPtr<UBoxComponent> RightBoxComponent;

	/* Current Direction of the movement (we need it when we want to know what direction we should move next.)*/
	float Direction = 1.f;

	TEnumAsByte<EMovementDirection> MovementDirection = EMovementDirection::Right;

	TEnumAsByte<EEnemyMovementMode> MovementMode = EEnemyMovementMode::Freezed;

	UFUNCTION(BlueprintCallable)
		void SetMovementDirection(TEnumAsByte<EMovementDirection> NewMovementMode);

	UFUNCTION(BlueprintCallable)
		void SetMovementMode(TEnumAsByte < EEnemyMovementMode> NewMovementMode);

	UFUNCTION()
		void RightBoxOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void LeftBoxOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void Move(float DeltaTime);
	
	UFUNCTION()
		void SpawnAnEnemy(int EnemyTypeIndex, FVector SpawnLocation, FRotator SpawnRotation);

	UFUNCTION()
		void DestroyAllEnemies();

	UFUNCTION()
		void DelayMovmenet();

	UFUNCTION()
		void ResetMovement();

	UFUNCTION()
		void OnEnemyDestroyed(AEnemyBasePawn* Enemy);

	UFUNCTION()
		void UpdateEdgeScreenBoxes(int EnemyIndex);

	UFUNCTION()
		void UpdateMovementSpeed();

	UFUNCTION()
		void UpdateShooter(int EnemyIndex);

	UFUNCTION()
		void FireAtPlayer();

	UFUNCTION()
		void InitializeSpawner();

	UFUNCTION()
		void InitializeEdgeScreenBoxes();

	UFUNCTION()
		void SpawnAllEnemies();

	TObjectPtr<AEnemyBasePawn> GetEnemy(int r, int c);
};


