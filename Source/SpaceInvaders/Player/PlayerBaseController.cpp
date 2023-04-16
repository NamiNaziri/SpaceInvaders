// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBaseController.h"
#include "PlayerBaseState.h"
#include "Kismet/GameplayStatics.h"
#include "SpaceInvaders/Game/CoreGameMode.h"
#include "SpaceInvaders/Game/CoreHUD.h"
void APlayerBaseController::AddScore(float ScoreToAdd)
{
	APlayerBaseState* PS = GetPlayerState<APlayerBaseState>();
	if (PS)
	{
		PS->AddScore(ScoreToAdd);
	}
}

void APlayerBaseController::SetHealth(float Health)
{
	APlayerBaseState* PS = GetPlayerState<APlayerBaseState>();
	if (PS)
	{
		PS->SetHealth(Health);
	}
}

void APlayerBaseController::SetMaxHealth(float MaxHealth)
{
	APlayerBaseState* PS = GetPlayerState<APlayerBaseState>();
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
