// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "CoreHUD.generated.h"

class UUserWidget;

/*
 * The class is designed to centralize the management of widgets. Other classes can call different functionality from this class.
 */
UCLASS()
class SPACEINVADERS_API ACoreHUD : public AHUD
{
	GENERATED_BODY()

public:

	virtual void BeginPlay() override;


	UFUNCTION(BlueprintCallable)
		void ClearFocusedWidget();

	UFUNCTION(BlueprintCallable)
		void InitFocusedWidget();

	UFUNCTION(BlueprintCallable)
		void ClearMainMenu();

	UFUNCTION(BlueprintCallable)
		void ShowPlayerMainHud();

	UFUNCTION(BlueprintCallable)
		void ClearPlayerMainHud();

	UFUNCTION(BlueprintCallable)
		void ShowPauseMenu();

	UFUNCTION(BlueprintCallable)
		void ClearPauseMenu();

	UFUNCTION(BlueprintCallable)
		void ShowLeaderboardEntryModel();

	UFUNCTION(BlueprintCallable)
		void ClearLeaderboardEntryModel();

protected:


	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "PlayerController")
		TObjectPtr<APlayerController> PlayerController;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "UserWidget")
		TSubclassOf<UUserWidget> MainMenuClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "UserWidget")
		TSubclassOf<UUserWidget> PauseMenuClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "UserWidget")
		TSubclassOf<UUserWidget> PlayerMainHudClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "UserWidget")
		TSubclassOf<UUserWidget> LeaderboardEntryModalClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "UserWidget")
		TSubclassOf<UUserWidget> StartLevelCountdownClass;



	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "UserWidget")
		TObjectPtr<UUserWidget> WBP_MainMenu;
		
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "UserWidget")
		TObjectPtr<UUserWidget> WBP_PauseMenu;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "UserWidget")
		TObjectPtr<UUserWidget> WBP_PlayerMainHud;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "UserWidget")
		TObjectPtr<UUserWidget> WBP_LeaderboardEntryModal;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "UserWidget")
		TObjectPtr<UUserWidget> WBP_StartLevelCountdown;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "UserWidget")
		TObjectPtr<UUserWidget> CurrentFocusedWidget;


	// Returns the new created widget
	UUserWidget* CreateWidgetAndAddToViewPort(TSubclassOf<UUserWidget> WidgetClass);
	
};
