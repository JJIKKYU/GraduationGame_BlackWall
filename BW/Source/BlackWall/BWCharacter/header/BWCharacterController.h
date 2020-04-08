// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BWCharacterController.generated.h"

/**
 * 
 */
UCLASS()
class BLACKWALL_API ABWCharacterController : public APlayerController
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
	TSubclassOf<class UUserWidget> HUDOverlayAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
	UUserWidget* HUDOverlay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
	TSubclassOf<UUserWidget> WEnemyHealthBar;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Widget")
	UUserWidget* EnemyHealthBar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
	TSubclassOf<UUserWidget> WPauseMenu;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Widget")
	UUserWidget* PauseMenu;

	bool bPauseMenuVisible;

	FVector EnemyLocation;

	bool bEnemyHealthBarVisible;
	
	//UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HUD")
	void DisplayPauseMenu();

	//UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HUD")
	void RemovePauseMenu();
	void TogglePauseMenu();	

	void DisplayEnemyHealthBar();

	void RemoveEnemyHealthBar();


	void GameModeOnly();

	/**
	* ComboCnt
	*/
	int comboCnt;

	void ComboCntManager();
	int GetComboCnt();
	void SetComboCnt(int comboCnt);


	/**
	* Tutorial
	*/

	/**
	* Score
	* 콤보 점수를 스코어로 환산해서 저장
	*/
	int score;

	void ScoreManager();
	void SetScore(int score);
	int GetScore();



protected:

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	
};