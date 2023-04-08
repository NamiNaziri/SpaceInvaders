// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Delegates/Delegate.h"
#include "EnemyBaseActor.generated.h"

class UBoxComponent;
class AProjectileBaseActor;
class AProjectileLauncher;

UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemyHit, AEnemyBaseActor*, HitEnemy);

UCLASS()
class SPACEINVADERS_API AEnemyBaseActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemyBaseActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FOnEnemyHit OnEnemyHit;

	const bool& IsEnemyEnable();
	void SetEnemyEnable(bool bIsEnabled);


	UFUNCTION()
		void Shoot();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		TObjectPtr<UBoxComponent> BoxComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = Shooting)
		TSubclassOf<AProjectileLauncher> ProjectileLauncherClass;

	UPROPERTY()
		TObjectPtr<AProjectileLauncher> ProjectileLauncher;



	bool bIsEnemyEnabled = true;

	UFUNCTION()
		void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


	void InitProjectileLauncher();
};
