// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelTransitionVolume.h"
#include "Components/BoxComponent.h"
#include "Components/BillboardComponent.h"
#include "BlackWallCharacter.h"
#include "Weapon.h"


// Sets default values
ALevelTransitionVolume::ALevelTransitionVolume()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	mTransitionVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("TransitionVolume"));
	RootComponent = mTransitionVolume;

	mBillboard = CreateDefaultSubobject<UBillboardComponent>(TEXT("Billboard"));
	mBillboard->SetupAttachment(GetRootComponent());

	TransitionLevelName = "";
}

// Called when the game starts or when spawned
void ALevelTransitionVolume::BeginPlay()
{
	Super::BeginPlay();

	mTransitionVolume->OnComponentBeginOverlap.AddDynamic(this, &ALevelTransitionVolume::OnOverlapBegin);
}

// Called every frame
void ALevelTransitionVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALevelTransitionVolume::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		ABlackWallCharacter* BWCharacter = Cast<ABlackWallCharacter>(OtherActor);
		if (BWCharacter)
		{
			//BWCharacter->SwitchLevel(TransitionLevelName);
			BWCharacter->EquippedWeapon->SwitchLevel(TransitionLevelName);
		}
	}
}