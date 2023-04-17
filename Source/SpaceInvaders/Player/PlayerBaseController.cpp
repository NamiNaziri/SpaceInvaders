// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBaseController.h"
#include "Kismet/GameplayStatics.h"

#include "SpaceInvaders/Game/CoreGameMode.h"
#include "SpaceInvaders/Game/CoreHUD.h"
#include "SpaceInvaders/Game/CorePlayerState.h"

void APlayerBaseController::AddScore(float ScoreToAdd)
{
	ACorePlayerState* PS = GetPlayerState<ACorePlayerState>();
	if (PS)
	{
		PS->AddScore(ScoreToAdd);
	}
}

void APlayerBaseController::SetHealth(float Health)
{
	ACorePlayerState* PS = GetPlayerState<ACorePlayerState>();
	if (PS)
	{
		PS->SetHealth(Health);
	}
}

void APlayerBaseController::SetMaxHealth(float MaxHealth)
{
	ACorePlayerState* PS = GetPlayerState<ACorePlayerState>();
	if (PS)
	{
		PS->SetMaxHealth(MaxHealth);
	}
}

void APlayerBaseController::RecieveOnDeath()
{
	UGameplayStatics::SetGamePaused(GetWorld(), true);
	TObjectPtr<ACoreGameMode> CGM = Cast<ACoreGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (CGM)
	{
		CGM->PlayerDied();
	}
	TObjectPtr < ACoreHUD> CoreHUD = Cast<ACoreHUD>(GetHUD());
	if (CoreHUD)
	{
		CoreHUD->ShowLeaderboardEntryModel();
	}

}
