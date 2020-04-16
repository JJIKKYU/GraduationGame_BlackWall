// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Info.h"
#include "Quest.generated.h"

/**
 * 
 */
UCLASS()
class BLACKWALL_API AQuest : public AInfo
{
	GENERATED_BODY()
	
public:
    // Sets default values for this actor's properties
    AQuest();

    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    // Called every frame
    virtual void Tick(float DeltaSeconds) override;

public:

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Q")
    TArray<class AMyInfo*> CurrentObjectives;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Q")
    TArray<TSubclassOf<AMyInfo>> Objectives;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Q")
    class USoundCue* QuestStartSoundCue;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Q")
    FName QuestName;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Q")
    FText QuestStartDescription;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Q")
    FText QuestEndDescription;

    UFUNCTION(BlueprintCallable, Category = "Q")
    bool IsQuestComplete() const;

    UFUNCTION(BlueprintCallable, Category = "Q")
    bool CanUpdate(FName Objective);

    UFUNCTION(BlueprintCallable, Category = "Q")
    void Update(FName Objective, int32 Progress);

    UFUNCTION(BlueprintCallable, Category = "Q")
    bool TryUpdate(FName Objective, int32 Progress);

    UFUNCTION(BlueprintCallable, Category = "Q")
    float QuestCompletion() const;
};
