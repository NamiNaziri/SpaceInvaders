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

	UPROPERTY()
	TObjectPtr<AProjectileLauncher> ProjectileLauncher;

	UFUNCTION()
		void Move(const FInputActionInstance& Instance);

	UFUNCTION()
		void Shoot(const FInputActionInstance& Instance);

	UFUNCTION()
		void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


	void InitProjectileLauncher();
};
