// Fill out your copyright notice in the Description page of Project Settings.


#include "Timer.h"
#include "Engine/Engine.h"
#include "EngineUtils.h"

// Sets default values
ATimer::ATimer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bEndOfTimer = false;
	bIsStart = false;
	timerCount = 0.f;
}

// Called when the game starts or when spawned
void ATimer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATimer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (timerCount > 0 && bIsStart)
	{
		timerCount -= DeltaTime;
		GEngine->AddOnScreenDebugMessage(0, 0, FColor::Green, FString::SanitizeFloat(timerCount));

	}
	else
	{
		bIsStart = false;
		bEndOfTimer = true;
	}
}

