// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGameInstance.h"
#include "Engine/World.h"
#include "Engine/Engine.h"
#include "../Quest/QuestSystem.h"
#include "EngineUtils.h"


UMainGameInstance::UMainGameInstance()
{
    FQuestSystem TutorialSystem1 = { 0, false, TEXT("-[ ] 콘솔을 이용해 앞으로 이동하세요."), "empty", "tutorialMonster", 1 };

    questSystem.Add(TutorialSystem1);
    
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

    

    UE_LOG(LogTemp, Warning, TEXT("Init, path = %s"), *GetPathName());
}

void UMainGameInstance::StartGameInstance()
{
    Super::StartGameInstance();

    UE_LOG(LogTemp, Warning, TEXT("StartGameInstance, path = %s"), *GetPathName());
}

void UMainGameInstance::OnStart()
{
    Super::OnStart();


    UE_LOG(LogTemp, Warning, TEXT("OnStart, path = %s"), *GetPathName());

    // FQuestSystem a{ 1, true, "abc", "abc","bac", "b" };

    int numActors = 0;
    for (TActorIterator<AActor> ActorItr(GetWorld()); ActorItr; ++ActorItr)
    {
        if (ActorItr->ActorHasTag(TEXT("Monster1")))
            GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, ActorItr->GetName());
        /*
        if (GEngine)
            GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, ActorItr->GetName());
            */

        // 출력 로그에 로그를 기록합니다.
        // UE_LOG(LogTemp, Log, TEXT("%s"), *(ActorItr->GetName()));

        ++numActors;
    }

    GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, FString::FromInt(numActors));

    // UE_LOG(LogTemp, Warning, TEXT("QuestSystem = %c"), questSystemTest.questTitle);
}





///////////////////////////////////////////////// TimerManager


UTimerManager::UTimerManager()
{
    
}

void UTimerManager::CallTimer()
{
    FTimerHandle tHandle;
    const float Delay = 1.0f;
    GetWorld()->GetTimerManager().SetTimer(tHandle, this, &UTimerManager::TestTimer, Delay, false);
}

void UTimerManager::TestTimer()
{
    GEngine->AddOnScreenDebugMessage(1, 2, FColor::Red, "Hello World");
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


///////////////////////////////////////////////// QuestManager

UQuestManager::UQuestManager()
{
    UE_LOG(LogTemp, Warning, TEXT("QuestManager Init"));
}

void UQuestManager::Tick(float DeltaTime)
{

}

void UQuestManager::QuestClear(FQuestSystem* quest)
{
    quest->bIsClear = true;
}

TStatId UQuestManager::GetStatId() const
{
    return TStatId();
}


UWorld* UQuestManager::GetWorld() const
{
    return GetOuter()->GetWorld();
}