// Fill out your copyright notice in the Description page of Project Settings.


#include "MyInfo.h"

// Sets default values
AMyInfo::AMyInfo() :
    Description(),
    ObjectiveName(NAME_None),
    TotalProgressNeeded(1),
    CurrentProgress(0),
    MustBeCompletedToAdvance(true)
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMyInfo::BeginPlay()
{
    Super::BeginPlay();
}

// Called every frame
void AMyInfo::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}

void AMyInfo::Update(int32 Progress)
{
    CurrentProgress += Progress;
}

bool AMyInfo::IsComplete() const
{
    return CurrentProgress >= TotalProgressNeeded;
}

float AMyInfo::GetProgress() const
{
    check(TotalProgressNeeded != 0)
        return (float)CurrentProgress / (float)TotalProgressNeeded;
}