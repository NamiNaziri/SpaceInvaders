// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawner.generated.h"

class UBoxComponent;
class AEnemyBaseActor;

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
		TArray<TSubclassOf<AEnemyBaseActor>> EnemyType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), category = EnemySetting)
		TArray<TObjectPtr<AEnemyBaseActor>> Enemies;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), category = Movement)
		float MovementSpeed = 1.f;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), category = Movement)
		int32 maxHeightLevel = 10;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), category = Movement)
		int32 currentHeightLevel = 0;




	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBoxComponent> LeftBoxComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBoxComponent> RightBoxComponent;

	bool bRightBoxOverlapped = false;

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



};


