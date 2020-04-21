// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "QuestSystemManager.generated.h"

/**
 * 
 */
UCLASS()
class BLACKWALL_API UQuestSystemManager : public UObject
{
	GENERATED_BODY()
	class UMainGameInstance* MainGameInstance;

public:

	UQuestSystemManager();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quset")
	class UDataTable* questTable;

	int MonsterCountByTag(UWorld* world, FName monsterTag);

	
	

	void Test();
};
