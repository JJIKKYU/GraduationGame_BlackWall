// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestSystemManager.h"
#include "../GameInstnace/MainGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/World.h"
#include "Engine/Engine.h"
#include "EngineUtils.h"
#include "../Quest/QuestSystem.h"

UQuestSystemManager::UQuestSystemManager()
{
    // MainGameInstance = UMainGameInstance::GetInstance();
    questTable = LoadObject<UDataTable>(nullptr, TEXT("DataTable'/Game/9b_DataTable/TutorialQuestTable.TutorialQuestTable'"), nullptr, LOAD_None, nullptr);
}

int UQuestSystemManager::MonsterCountByTag(UWorld* world ,FName monsterTag)
{
    int monsterNumber = 0;
    FWorldContext* context = GEngine->GetWorldContextFromGameViewport(GEngine->GameViewport);
    
    for (TActorIterator<AActor> ActorItr(world); ActorItr; ++ActorItr)
    {
        if (ActorItr->ActorHasTag(monsterTag))
        {
            // GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, ActorItr->GetName());
            ++monsterNumber;
        }
    }

    UE_LOG(LogTemp, Warning, TEXT("Counts = %d"), monsterNumber);

    return monsterNumber;
}

void UQuestSystemManager::Test()
{
    // questTable = LoadObject<UDataTable>(nullptr, TEXT("DataTable'/Game/9b_DataTable/TutorialQuestTable.TutorialQuestTable'"), nullptr, LOAD_None, nullptr);

    /*
    if (questTable)
    {
        UE_LOG(LogTemp, Warning, TEXT("ttest"));

        auto Row = questTable->FindRow<FQuestTable>(FName("1"), FString(""));
        if (Row)
        {
            GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, *Row->Description);
        }

    }
    */

    UE_LOG(LogTemp, Warning, TEXT("test"));
}
