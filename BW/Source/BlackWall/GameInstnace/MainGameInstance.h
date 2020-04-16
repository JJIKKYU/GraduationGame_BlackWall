// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Tickable.h"
#include "../Quest/QuestSystem.h"
#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MainGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class BLACKWALL_API UMainGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:

	UMainGameInstance();

	virtual void Init() override;

	virtual void StartGameInstance() override;

	virtual void OnStart() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BWCharacter")
	class ABlackWallCharacter* BWCharacter;

	/*
	* BWCharacter Level System
	*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
	int level;

	UFUNCTION(BlueprintCallable)
	int GetLevel();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	TArray<FQuestSystem> questSystem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	FQuestSystem questSystemTest;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	int aSystem;

	/**
	* Äù½ºÆ® ¸ñ·Ï
	*/
	void QuestInit();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	FQuestSystem tutorialQuest1;
};


UCLASS()
class BLACKWALL_API UTimerManager : public UObject, public FTickableGameObject
{
	GENERATED_BODY()

public:
	UTimerManager();
	UFUNCTION(BlueprintCallable, Category = Test)
		void CallTimer();

	void TestTimer();

	void Tick(float DeltaTime) override;
	bool IsTickable() const override;
	bool IsTickableInEditor() const override;
	bool IsTickableWhenPaused() const override;
	TStatId GetStatId() const override;

	UWorld* GetWorld() const override;

	float TestCounter;
};

UCLASS()
class BLACKWALL_API UQuestManager : public UObject, public FTickableGameObject
{
	GENERATED_BODY()

public:
	UQuestManager();

	void Tick(float DeltaTime) override;
	/*
	bool IsTickable() const override;
	bool IsTickableInEditor() const override;
	bool IsTickableWhenPaused() const override;
	*/

	void QuestClear(FQuestSystem* quest);

	TStatId GetStatId() const override;
	UWorld* GetWorld() const override;
};