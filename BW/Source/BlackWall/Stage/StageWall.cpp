// Fill out your copyright notice in the Description page of Project Settings.


#include "StageWall.h"
#include "Engine/World.h"
#include "Engine/Engine.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values
AStageWall::AStageWall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bCanOpen = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void AStageWall::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AStageWall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CheckStageMonster();

	if (monsterCount == 0 && bCanOpen == false)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, TEXT("Monster is gone~ Open the door"));
		bCanOpen = true;
		Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AStageWall::CheckStageMonster()
{
	FWorldContext* context = GEngine->GetWorldContextFromGameViewport(GEngine->GameViewport);

	monsterCount = 0;
	for (TActorIterator<AActor> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		if (ActorItr->ActorHasTag(monsterTag))
		{
			// GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, ActorItr->GetName());
			++monsterCount;
		}
	}
}