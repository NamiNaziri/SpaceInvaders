// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayerBaseController.generated.h"

/**
 * 
 */
UCLASS()
class SPACEINVADERS_API APlayerBaseController : public APlayerController
{
	GENERATED_BODY()

public:

	void AddScore(float ScoreToAdd);
	void SetHealth(float Health);
	void SetMaxHealth(float MaxHealth);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Death")
		void RecieveOnDeath();
	
};
