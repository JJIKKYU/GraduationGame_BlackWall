// Fill out your copyright notice in the Description page of Project Settings.


#include "StageClearPortal.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/BoxComponent.h"
#include "Engine/World.h"
#include "Engine/Engine.h"
#include "EngineUtils.h"
#include "Blueprint/UserWidget.h"
#include "../BWCharacter/header/BlackWallCharacter.h"

// Sets default values
AStageClearPortal::AStageClearPortal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PrimaryActorTick.bCanEverTick = true;

	triggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	RootComponent = triggerBox;

	triggerBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	triggerBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	triggerBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	triggerBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	triggerBox->SetBoxExtent(FVector(62.f, 62.f, 62.f));
}

// Called when the game starts or when spawned
void AStageClearPortal::BeginPlay()
{
	Super::BeginPlay();

	monsterCount = 0;

	triggerBox->OnComponentBeginOverlap.AddDynamic(this, &AStageClearPortal::OnOverlapBegin);
	triggerBox->OnComponentEndOverlap.AddDynamic(this, &AStageClearPortal::OnOverlapEnd);

	if (tutorialWidget)
	{
		tutorialWidget->AddToViewport();
		tutorialWidget->SetVisibility(ESlateVisibility::Hidden);
	}
	
}

// Called every frame
void AStageClearPortal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    

}


void AStageClearPortal::CheckStageMonster(FText monsterTag)
{
	FWorldContext* context = GEngine->GetWorldContextFromGameViewport(GEngine->GameViewport);

	monsterCount = 0;
	for (TActorIterator<AActor> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		if (ActorItr->ActorHasTag("monster_stage_1"))
		{
			// GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, ActorItr->GetName());
			++monsterCount;
		}
	}

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Check Monster Counts = %d"), monsterCount));
}


void AStageClearPortal::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor) return;

	BWCharacter = Cast<ABlackWallCharacter>(OtherActor);
	if (BWCharacter)
	{
		CheckStageMonster(monsterName);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Check Stage Clear Portal")));
		if (monsterCount > 0)
		{
			if (tutorialWidget)
			{
				tutorialWidget->SetVisibility(ESlateVisibility::Visible);
			}
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Nothing Monster! Clear Stage")));
			UGameplayStatics::OpenLevel(this, resultLevelName);
		}
	}
}

void AStageClearPortal::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Check Out Stage Clear Portal")));

	if (tutorialWidget)
	{
		tutorialWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}
