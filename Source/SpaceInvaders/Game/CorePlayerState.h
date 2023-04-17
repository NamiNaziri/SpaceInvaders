// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Delegates/Delegate.h"
#include "CorePlayerState.generated.h"

UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHelthChanged, float, NewHealth, float, MaxHealth);

UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnScoreChanged, float, NewScore);

UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxHealthChanged, float, NewMaxHealth);

/**
 * 
 */
UCLASS()
class SPACEINVADERS_API ACorePlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintCallable)
		void AddScore(float ScoreToAdd);

	UFUNCTION(BlueprintCallable)
		void SetMaxHealth(float NewMaxHealth);

	UFUNCTION(BlueprintCallable)
		float GetMaxHealth();
	
	UFUNCTION(BlueprintCallable)
		void SetHealth(float NewHealth);

	UPROPERTY(BlueprintAssignable)
		FOnHelthChanged OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
		FOnScoreChanged OnScoreChanged;

	UPROPERTY(BlueprintAssignable)
		FOnMaxHealthChanged OnMaxHealthChanged;
protected:

	float Health = 3;
	float MaxHealth = 3;
};
