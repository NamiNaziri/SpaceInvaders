// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PlayerBasePawn.generated.h"

class UInputMappingContext;
class UInputAction;
class UBoxComponent;
class UPawnMovementComponent;
class AProjectileBaseActor;
class AProjectileLauncher;


/*
*	TimerBased: The fire rate depends on timer. for example, player can shoot evert X seconds.	
*	AvailabilityBased:  The fire rate depend on whether the projectile is available or not.
*						The projectile become avialable when the previous instanc has hit an object( can be enemy, enemy projectile, wall, etc)
*						It uses the poolobject bShouldCreateNew variable. The PoolCapacity usually is set to 1 in this option.
*/

UENUM(BlueprintType)
enum EFireRateMode
{
	TimerBased UMETA(DisplayName = "TimerBased"), 
	AvailabilityBased UMETA(DisplayName = "AvailabilityBased") 
};


UCLASS()
class SPACEINVADERS_API APlayerBasePawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlayerBasePawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


protected:


	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = Movement)
		float MovementSpeed = 20.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = Projectile)
	TSubclassOf<AProjectileBaseActor> ProjectileType;

	UPROPERTY(Category = Pawn, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		TObjectPtr<UPawnMovementComponent> MovementComponent;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		TObjectPtr<UBoxComponent> BoxComponent;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = Input)
	TObjectPtr<UInputMappingContext> Default_KBM_MappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = Input)
		TObjectPtr<UInputAction> MoveInputAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = Input)
		TObjectPtr<UInputAction> ShootInputAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = Shooting)
		TSubclassOf<AProjectileLauncher> ProjectileLauncherClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = Shooting)
		TEnumAsByte<EFireRateMode> FireRateMode= EFireRateMode::AvailabilityBased;

	// Rate at which a weapon can fire projectiles. 1 shot per second.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = Shooting)
		float FireRate = 1.f;

	FTimerHandle TimerHandle_CanShoot;

	bool bCanShoot = true;

	UPROPERTY()
	TObjectPtr<AProjectileLauncher> ProjectileLauncher;

	UFUNCTION()
		void Move(const FInputActionInstance& Instance);

	UFUNCTION()
		void Shoot(const FInputActionInstance& Instance);

	UFUNCTION()
		void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void CanShoot();

	void InitProjectileLauncher();
};
