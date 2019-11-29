// Fill out your copyright notice in the Description page of Project Settings.
#include <EngineGlobals.h>
#include <Runtime/Engine/Classes/Engine/Engine.h>

#include "TutorialCheckpoint.h"
#include "Components/BoxComponent.h"
#include "BlackWallCharacter.h"
#include "Blueprint/UserWidget.h"

// Sets default values
ATutorialCheckpoint::ATutorialCheckpoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	mTriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	RootComponent = mTriggerBox;

	mTriggerBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	mTriggerBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	mTriggerBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	mTriggerBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	mTriggerBox->SetBoxExtent(FVector(62.f, 62.f, 62.f));
}

// Called when the game starts or when spawned
void ATutorialCheckpoint::BeginPlay()
{
	Super::BeginPlay();

	if (mTutorialWidget)
	{
		mTutorialWidget->AddToViewport();
		mTutorialWidget->SetVisibility(ESlateVisibility::Hidden);
	}
	
	mTriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ATutorialCheckpoint::OnOverlapBegin);
	mTriggerBox->OnComponentEndOverlap.AddDynamic(this, &ATutorialCheckpoint::OnOverlapEnd);
}

// Called every frame
void ATutorialCheckpoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATutorialCheckpoint::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor) return;
	ABlackWallCharacter* BWCharacter = Cast<ABlackWallCharacter>(OtherActor);
	if (BWCharacter)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("OnOverlapBegin_TutorialCheckPoint")));
		if (mTutorialWidget)
		{
			mTutorialWidget->SetVisibility(ESlateVisibility::Visible);
		}
		
	}
}

void ATutorialCheckpoint::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("OnOverlapEnd_TutorialCheckPoint")));
	if (mTutorialWidget)
	{
		mTutorialWidget->SetVisibility(ESlateVisibility::Hidden);
	}
	
}

