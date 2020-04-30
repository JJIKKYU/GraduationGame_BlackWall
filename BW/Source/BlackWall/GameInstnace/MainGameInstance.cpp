// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGameInstance.h"
#include "Engine/World.h"
#include "Engine/Engine.h"
#include "../Quest/QuestSystem.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "Containers/Ticker.h"
#include "TimerManager.h"



UMainGameInstance* UMainGameInstance::GetInstance()
{
    UMainGameInstance* instance = nullptr;

    if (GEngine)
    {
        FWorldContext* context = GEngine->GetWorldContextFromGameViewport(GEngine->GameViewport);
        instance = Cast<UMainGameInstance>(context->OwningGameInstance);
    }
    return instance;
}

int UMainGameInstance::GetLevel()
{
	return level;
}

void UMainGameInstance::QuestInit()
{
    // Tutorial Quest
    
}

void UMainGameInstance::Init()
{
    Super::Init();
    
    totalScore = 0;

    // Stage 클리어 카운터
    stageClearCounts = 0;


    TickDelegateHandle = FTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateUObject(this, &UMainGameInstance::Tick));

    UE_LOG(LogTemp, Warning, TEXT("Init, path = %s"), *GetPathName());
}

void UMainGameInstance::OnStart()
{
    Super::OnStart();

    QuestManager = LoadObject<UQuestSystemManager>(nullptr, TEXT("Class'/Script/BlackWall.QuestSystemManager'"), nullptr, LOAD_None, nullptr);
    QuestManager->Test();
    QuestManager->MonsterCountByTag(GetWorld(), TEXT("Monster1"));
  
    // MonsterSearch(TEXT("Monster1"));

    UE_LOG(LogTemp, Warning, TEXT("OnStart, path = %s"), *GetPathName());

    /*
    QuestTable = LoadObject<UDataTable>(nullptr, TEXT("DataTable'/Game/9b_DataTable/TutorialQuestTable.TutorialQuestTable'"), nullptr, LOAD_None, nullptr);
    if (QuestTable)
    {
        UE_LOG(LogTemp, Warning, TEXT("제발"));
        
        auto Row = QuestTable->FindRow<FQuestTable>(FName("1"), FString(""));
        if (Row)
        {
            GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "test99999");
            GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, *Row->Description);
        }

    }
    */
   
}

bool UMainGameInstance::Tick(float DeltaSeconds)
{
    QuestManager->MonsterCountByTag(GetWorld(), TEXT("Monster1"));

    return true;
}




///////////////////////////////////////////////// TimerManager


UTimerManager::UTimerManager()
{
}

void UTimerManager::Tick(float DeltaTime)
{
    TestCounter += DeltaTime;

    GEngine->AddOnScreenDebugMessage(0, 0, FColor::Green, FString::SanitizeFloat(TestCounter));

}

bool UTimerManager::IsTickable() const
{
    return true;
}

bool UTimerManager::IsTickableInEditor() const
{
    return false;
}

bool UTimerManager::IsTickableWhenPaused() const
{
    return false;
}

TStatId UTimerManager::GetStatId() const
{
    return TStatId();
}

UWorld* UTimerManager::GetWorld() const
{
    return GetOuter()->GetWorld();
}
