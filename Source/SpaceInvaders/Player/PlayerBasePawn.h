// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "../Pawn/BasePawn.h"
#include "PlayerBasePawn.generated.h"

class UInputMappingContext;
class UInputAction;
class USoundBase;
class APlayerProjectileLauncher;


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

	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Player|Sound Cue")
		TObjectPtr<USoundBase> ImpactSoundCue;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Player|Movement")
		float MovementSpeed = 20.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Player|Movement")
		TObjectPtr<UPawnMovementComponent> MovementComponent;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Player|Input")
		TObjectPtr<UInputMappingContext> Default_KBM_MappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Player|Input")
		TObjectPtr<UInputAction> MoveInputAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Player|Input")
		TObjectPtr<UInputAction> ShootInputAction;

	UPROPERTY()
		TObjectPtr<APlayerProjectileLauncher> PlayerProjectileLauncher;


	virtual void TakePointDamage(AActor* DamagedActor, float Damage, class AController* InstigatedBy, FVector HitLocation, class UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const class UDamageType* DamageType, AActor* DamageCauser);

	virtual void HealthBecomeZero(AActor* OwnerActor) override;

	UFUNCTION()
		void Move(const FInputActionInstance& Instance);

	UFUNCTION()
		void Shoot(const FInputActionInstance& Instance);

	virtual void InitProjectileLauncher() override;
};
