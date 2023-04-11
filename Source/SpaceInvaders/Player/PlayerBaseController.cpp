// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBaseController.h"
#include "PlayerBaseState.h"

void APlayerBaseController::AddScore(float ScoreToAdd)
{
	APlayerBaseState* PS = GetPlayerState<APlayerBaseState>();
	PS->AddScore(ScoreToAdd);
}

void APlayerBaseController::SetHealth(float Health)
{
	APlayerBaseState* PS = GetPlayerState<APlayerBaseState>();
	PS->SetHealth(Health);
}

void APlayerBaseController::SetMaxHealth(float MaxHealth)
{
	APlayerBaseState* PS = GetPlayerState<APlayerBaseState>();
	PS->SetMaxHealth(MaxHealth);
}
