// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnLocationActor.generated.h"

/*
*	This actor is utilized to establish spawn locations that can be used in the game mode for spawning actors.
*	To obtain the spawn locations in world position,  the location of the actor should be set as the origin of the world.
*/

UCLASS()
class SPACEINVADERS_API ASpawnLocationActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnLocationActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameMode|Spawner", Meta = (MakeEditWidget = true))
		FVector EnemySpawnerLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameMode|Spawner", Meta = (MakeEditWidget = true))
		FVector UFOSpawnerLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameMode|Player", Meta = (MakeEditWidget = true))
		FVector PlayerLocation;

};
