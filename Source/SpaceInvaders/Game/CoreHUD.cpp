// Fill out your copyright notice in the Description page of Project Settings.


#include "CoreHUD.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

void ACoreHUD::BeginPlay()
{
	Super::BeginPlay();
	this->PlayerController = GetOwningPlayerController();

	WBP_MainMenu = CreateWidgetAndAddToViewPort(MainMenuClass);
	WBP_PlayerMainHud = CreateWidgetAndAddToViewPort(PlayerMainHudClass);
	WBP_PauseMenu = CreateWidgetAndAddToViewPort(PauseMenuClass);
	WBP_LeaderboardEntryModal = CreateWidgetAndAddToViewPort(LeaderboardEntryModalClass);
	WBP_StartLevelCountdown = CreateWidgetAndAddToViewPort(StartLevelCountdownClass);

	CurrentFocusedWidget = WBP_MainMenu;
	InitFocusedWidget();
}


void ACoreHUD::ClearMainMenu()
{
	ShowPlayerMainHud();
	ClearFocusedWidget();
}

void ACoreHUD::ClearFocusedWidget()
{
	PlayerController->SetShowMouseCursor(false);
	UWidgetBlueprintLibrary::SetInputMode_GameOnly(PlayerController);

	if (CurrentFocusedWidget)
	{
		CurrentFocusedWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("CurrentFocusedWidget is not set"));
	}

}

void ACoreHUD::InitFocusedWidget()
{
	PlayerController->SetShowMouseCursor(true);
	UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(PlayerController, CurrentFocusedWidget);

	if (CurrentFocusedWidget)
	{
		CurrentFocusedWidget->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("CurrentFocusedWidget is not set"));
	}
	
}

void ACoreHUD::ShowPlayerMainHud()
{
	if (WBP_PlayerMainHud)
	{
		WBP_PlayerMainHud->SetVisibility(ESlateVisibility::Visible);
	}
	
}

void ACoreHUD::ClearPlayerMainHud()
{
	if (WBP_PlayerMainHud)
	{
		WBP_PlayerMainHud->SetVisibility(ESlateVisibility::Collapsed);
	}
	
}

void ACoreHUD::ShowPauseMenu()
{
	ClearPlayerMainHud();
	ClearFocusedWidget();
	CurrentFocusedWidget = WBP_PauseMenu;
	InitFocusedWidget();
}

void ACoreHUD::ClearPauseMenu()
{
	ShowPlayerMainHud();
	ClearFocusedWidget();
}

void ACoreHUD::ShowLeaderboardEntryModel()
{
	ClearPlayerMainHud();
	ClearFocusedWidget();
	CurrentFocusedWidget = WBP_LeaderboardEntryModal;
	InitFocusedWidget();
}

void ACoreHUD::ClearLeaderboardEntryModel()
{
	ShowPlayerMainHud();
	ClearFocusedWidget();
}


UUserWidget* ACoreHUD::CreateWidgetAndAddToViewPort(TSubclassOf<UUserWidget> WidgetClass)
{
	if (WidgetClass)
	{
		UUserWidget* newWidget = CreateWidget(PlayerController, WidgetClass);
		newWidget->AddToViewport();
		newWidget->SetVisibility(ESlateVisibility::Collapsed);
		return newWidget;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ACoreHUD::CreateWidgetAndAddToViewPort: WidgetClass not found"));
		return nullptr;
	}
}
