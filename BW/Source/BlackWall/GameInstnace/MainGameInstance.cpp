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
#include "../Item/Weapon/header/Weapon.h"



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

void UMainGameInstance::Init()
{
    Super::Init();
    
    totalScore = 0;
    comboCounter = 10.f;
    bComboCount = false;
    bActiveCounterHandle = false;
    level = 1;

    // Stage 클리어 카운터
    stageClearCounts = 0;
    stageClearCounts_FNAME = FName("1");


    TickDelegateHandle = FTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateUObject(this, &UMainGameInstance::Tick));

    UE_LOG(LogTemp, Warning, TEXT("Init, path = %s"), *GetPathName());
}

void UMainGameInstance::OnStart()
{
    Super::OnStart();

    //QuestManager = LoadObject<UQuestSystemManager>(nullptr, TEXT("Class'/Script/BlackWall.QuestSystemManager'"), nullptr, LOAD_None, nullptr);
    //QuestManager->Test();
    //QuestManager->MonsterCountByTag(GetWorld(), TEXT("Monster1"));
  
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
    // QuestManager->MonsterCountByTag(GetWorld(), TEXT("Monster1"));

    GEngine->AddOnScreenDebugMessage(0, 0, FColor::White, FString::Printf(TEXT("comboCounter = %f"), GetWorld()->GetTimerManager().GetTimerRemaining(ComboTimerHandle)));

    if (stageClearCounts == 0)
        stageClearCounts_FNAME = FName("1");
    else if (stageClearCounts == 1)
        stageClearCounts_FNAME = FName("2");
    else if (stageClearCounts >= 2)
        stageClearCounts_FNAME = FName("3");

    if (!bActiveCounterHandle)
    {
        bActiveCounterHandle = true;
        GetWorld()->GetTimerManager().SetTimer(ComboTimerHandle, this, &UMainGameInstance::testString, 5.f, false);
       
    }
    
    if (bComboCount)
    {
        if (comboCounter > 0.f)
        {
            
        }
        else if (comboCounter <= 0.f)
        {
            totalScore = comboCnt * 10;
            GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::White, FString::Printf(TEXT("totalScore = %d"), totalScore));
            bComboCount = false;
        }
    }
    

    return true;
}

int UMainGameInstance::ScoreCalculation()
{
    return 1000;
}

void UMainGameInstance::SetComboCnt(int value)
{
    bComboCount = true;
    GetWorld()->GetTimerManager().ClearTimer(ComboTimerHandle);
    bActiveCounterHandle = false;
    comboCounter = 10.f;

    comboCnt = value;
}

void UMainGameInstance::QuestInit()
{
    // Tutorial Quest

}



///////////////////////////////////////////////// TimerManager

UTimerManager::UTimerManager()
{
}

void UTimerManager::Tick(float DeltaTime)
{
    TestCounter += DeltaTime;

    // GEngine->AddOnScreenDebugMessage(0, 0, FColor::Green, FString::SanitizeFloat(TestCounter));

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
