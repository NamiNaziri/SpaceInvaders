// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayerBaseController.generated.h"

/*
 *	Player controller handles overal state of the player pawn.
 *	Communicate with other core classes such as player state and HUD class.
 *  Handles non-gamplay inputs such as pausing and show the related HUD based on them.
 */
UCLASS()
class SPACEINVADERS_API APlayerBaseController : public APlayerController
{
	GENERATED_BODY()

public:

	UFUNCTION()
		void AddScore(float ScoreToAdd);

	UFUNCTION()
		void SetHealth(float Health);

	UFUNCTION()
		void SetMaxHealth(float MaxHealth);

	UFUNCTION()
		void RecieveOnDeath();
	
};
