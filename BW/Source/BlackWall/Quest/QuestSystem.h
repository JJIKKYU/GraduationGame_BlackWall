// Fill out your copyright notice in the Description page of Project Settings.

/**
* 퀘스트 기본 구조
*/

#pragma once

#include "CoreMinimal.h"
#include "QuestSystem.generated.h"

USTRUCT(BlueprintType)
struct FQuestSystem
{
	GENERATED_USTRUCT_BODY()

public:
	FQuestSystem(int32 questNumber = 0, bool bIsClear = false, FString questTitle = "This is QuestTitle.",
		FString questDescription = "This is Quest Description.", FString targetMonsterTag = "Monster1",
		int32 targetMonsterCount = 3)
	{
		this->questNumber = questNumber;
		this->bIsClear = bIsClear;
		this->questTitle = questTitle;
		this->questDescription = questDescription;
		this->targetMonsterTag = targetMonsterTag;
		this->targetMonsterCount = targetMonsterCount;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 questNumber;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsClear;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString questTitle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString questDescription;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString targetMonsterTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 targetMonsterCount;
};