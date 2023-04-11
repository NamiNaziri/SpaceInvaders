// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBaseState.h"

void APlayerBaseState::AddScore(float ScoreToAdd)
{
	float CurrentScore = GetScore();
	SetScore(CurrentScore + ScoreToAdd);
	OnScoreChanged.Broadcast(CurrentScore + ScoreToAdd);
}

void APlayerBaseState::SetMaxHealth(float NewMaxHealth)
{
	this->MaxHealth = NewMaxHealth;
	OnMaxHealthChanged.Broadcast(NewMaxHealth);
	//this->Health = NewMaxHealth;
}

float APlayerBaseState::GetMaxHealth()
{
	return this->MaxHealth;
}

void APlayerBaseState::SetHealth(float NewHealth)
{
	this->Health = NewHealth;
	OnHealthChanged.Broadcast(this->Health, this->MaxHealth);
}
