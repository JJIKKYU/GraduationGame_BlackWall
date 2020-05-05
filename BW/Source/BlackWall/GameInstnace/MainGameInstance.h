// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Manager/QuestSystemManager.h"
#include "Tickable.h"
#include "../Quest/QuestSystem.h"
#include "../Data/DataTables.h"
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

	UMainGameInstance(){ };
	UQuestSystemManager* QuestManager;
	
public:

	static UMainGameInstance* GetInstance();

	void stringTest()
	{
		UE_LOG(LogTemp, Warning, TEXT("stringTestClear!"));
	}

	virtual void Init() override;

	virtual void OnStart() override;

	bool Tick(float DeltaSeconds);

	FDelegateHandle TickDelegateHandle;


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

	/*
	* Score 
	*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Score")
	int comboCnt;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Score")
	int totalScore;

	UFUNCTION(BlueprintCallable)
	int ScoreCalculation();

	void SetComboCnt(int value);
	FORCEINLINE int GetComboCnt() { return comboCnt; }

	float comboCounter;
	bool bComboCount;
	bool bActiveCounterHandle;
	FTimerHandle ComboTimerHandle;
	FTimerHandle ComboDownTimerHandle;
	void testString()
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::White, FString::Printf(TEXT("bomb")));
		totalScore += comboCnt * 10;
		GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::White, FString::Printf(TEXT("totalScore = %d"), totalScore));
		comboCnt = 0;
	}

	void down()
	{
		// if (comboCnt == 0) return;
		SetComboCnt(comboCnt--);
	}


	/**
	* 퀘스트 목록
	*/
	void QuestInit();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	FQuestSystem tutorialQuest1;

	FQuestTable* GetQuestTitle(int32 number)
	{
		return QuestTable->FindRow<FQuestTable>(*FString::FromInt(number), false);
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quset")
	class UDataTable* QuestTable;

	/*
	* Sound
	*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	class USoundBase* bgm;

	/*
	* Stage 클리어 여부
	*/

	// 스테이지 클리어 횟수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stage Clear")
	int stageClearCounts;

	UFUNCTION(BlueprintCallable)
	FORCEINLINE int GetStageClearCounts() { return stageClearCounts; }

	UFUNCTION(BlueprintCallable)
	void SetStageClearCounts(int value) { stageClearCounts = value; }
};


UCLASS()
class BLACKWALL_API UTimerManager : public UObject, public FTickableGameObject
{
	GENERATED_BODY()

public:
	UTimerManager();

	/*
	UFUNCTION(BlueprintCallable, Category = Test)
		void CallTimer();

	void TestTimer();
	*/

	void Tick(float DeltaTime) override;
	bool IsTickable() const override;
	bool IsTickableInEditor() const override;
	bool IsTickableWhenPaused() const override;
	TStatId GetStatId() const override;

	UWorld* GetWorld() const override;

	float TestCounter;
};