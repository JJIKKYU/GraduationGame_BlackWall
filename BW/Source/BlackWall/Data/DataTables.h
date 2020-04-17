// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "DataTables.generated.h"

USTRUCT(BlueprintType)
struct FQuestTable : public FTableRowBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	int32 Number;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
		bool bIsClear;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
		FString Title;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
		FString Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
		FString TargetMonsterTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
		FString TargetMonsterCount;

};