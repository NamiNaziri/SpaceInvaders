// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "../Pawn/BasePawn.h"
#include "PlayerBasePawn.generated.h"

class UInputMappingContext;
class UInputAction;


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
class SPACEINVADERS_API APlayerBasePawn : public ABasePawn
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

	UPROPERTY(Category = Pawn, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		TObjectPtr<UPawnMovementComponent> MovementComponent;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = Input)
	TObjectPtr<UInputMappingContext> Default_KBM_MappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = Input)
		TObjectPtr<UInputAction> MoveInputAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = Input)
		TObjectPtr<UInputAction> ShootInputAction;



	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = Shooting)
		TEnumAsByte<EFireRateMode> FireRateMode= EFireRateMode::AvailabilityBased;

	// Rate at which a weapon can fire projectiles. 1 shot per second.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = Shooting)
		float FireRate = 1.f;

	FTimerHandle TimerHandle_CanShoot;

	bool bCanShoot = true;

		virtual void TakePointDamage(AActor* DamagedActor, float Damage, class AController* InstigatedBy, FVector HitLocation, class UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const class UDamageType* DamageType, AActor* DamageCauser);

		virtual void HealthBecomeZero(AActor* OwnerActor) override;

	UFUNCTION()
		void Move(const FInputActionInstance& Instance);

	UFUNCTION()
		void Shoot(const FInputActionInstance& Instance);



	UFUNCTION()
		void CanShoot();

	virtual void InitProjectileLauncher() override;
};
