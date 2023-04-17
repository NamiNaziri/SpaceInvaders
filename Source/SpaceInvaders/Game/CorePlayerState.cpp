// Fill out your copyright notice in the Description page of Project Settings.


#include "CorePlayerState.h"

void ACorePlayerState::AddScore(float ScoreToAdd)
{
	float CurrentScore = GetScore();
	SetScore(CurrentScore + ScoreToAdd);
	OnScoreChanged.Broadcast(CurrentScore + ScoreToAdd);
}

void ACorePlayerState::SetMaxHealth(float NewMaxHealth)
{
	this->MaxHealth = NewMaxHealth;
	OnMaxHealthChanged.Broadcast(NewMaxHealth);
}

float ACorePlayerState::GetMaxHealth()
{
	return this->MaxHealth;
}

void ACorePlayerState::SetHealth(float NewHealth)
{
	this->Health = NewHealth;
	OnHealthChanged.Broadcast(this->Health, this->MaxHealth);
}
