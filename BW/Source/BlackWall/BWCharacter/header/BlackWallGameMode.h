// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "../../Quest/QuestSystem.h"
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BlackWallGameMode.generated.h"

UCLASS(minimalapi, BlueprintType)
class ABlackWallGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ABlackWallGameMode();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	TArray<FQuestSystem> questSystem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	FQuestSystem questSystemTest;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	FString questTitle;
};



