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


UCLASS()
class SPACEINVADERS_API AEnemySpawner : public AActor
{
	GENERATED_BODY()

		// the spawner is located on the top left of the enemies.


public:	
	// Sets default values for this actor's properties
	AEnemySpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void OnConstruction(const FTransform& Transform) override;

	void ResetSpawner(int Level);

	void PauseAllTimers();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), category = SpawnerSetting)
		int32 Row = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), category = SpawnerSetting)
		int32 Column= 2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), category = SpawnerSetting)
		float HorizontalStride = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), category = SpawnerSetting)
		float VerticalStride = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), category = SpawnerSetting)
		float boxMarginToScreenEnd = 1.f;



	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), category = EnemySetting)
		TArray<TSubclassOf<AEnemyBasePawn>> EnemyType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), category = EnemySetting)
		TArray<TObjectPtr<AEnemyBasePawn>> Enemies;

	// Row and Column of active an available shooters. 
	// <Key=Column, Value=Row>
	TMap<int32, int32> ActiveShooters;
	
	TArray<int32> DestroyedEnemiesPerColumn;

	int32 LeftBoxCurrentColumn;
	int32 RightBoxCurrentColumn;

	int RemainingEnemyCount;

	/*Base movement speed */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), category = Movement)
		float MovementSpeed = 400.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), category = Movement)
	/*Current Movement Speed*/
	float CurrentMovementSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), category = Movement)
		float MaxMovementSpeed = 1000.f;
	/*
	* This curve needs to be normalized between zero and one, as it will determine how the speed increases when an enemy is destroyed.
	* If the curve does not exist, a linear rate will be applied instead.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), category = Movement)
		UCurveFloat* SpeedChangeRateCurve;

	// wait after every DelayInterval(seconds)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), category = Movement)
		float DelayInterval = 1.f;



	// after the delay interval is reached, wait for DelayDuration(seconds)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), category = Movement)
		float DelayDuration = 1.f;


	FTimerHandle TimerHandle_DelayMovement;

	FTimerHandle TimerHandle_ResetMovement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), category = Movement)
		float VerticalMovementStride = 5;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), category = Movement)
		int32 maxHeightLevel = 10;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), category = Movement)
		int32 currentHeightLevel = 0;

	
	// Rate at which a weapon can fire projectiles. 1 shot per second.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = Shooting)
		float FireRate = 1.f;

	FTimerHandle TimerHandle_FireAtPlayer;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBoxComponent> LeftBoxComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBoxComponent> RightBoxComponent;

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



	TObjectPtr<AEnemyBasePawn> GetEnemy(int r, int c);
};


