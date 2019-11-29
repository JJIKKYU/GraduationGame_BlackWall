// Fill out your copyright notice in the Description page of Project Settings.


#include "FloorSwitch.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/Engine.h"
#include "TimerManager.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AFloorSwitch::AFloorSwitch()
	: mSwitchTime(2.f), bCharacterOnSwitch(false)
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

	mFloorSwitch = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FloorSiwtch"));
	mFloorSwitch->SetupAttachment(GetRootComponent());

	mDoor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door"));
	mDoor->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void AFloorSwitch::BeginPlay()
{
	Super::BeginPlay();
	
	mTriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AFloorSwitch::OnOverlapBegin);
	mTriggerBox->OnComponentEndOverlap.AddDynamic(this, &AFloorSwitch::OnOverlapEnd);

	mInitialDoorLocation = mDoor->GetComponentLocation();
	mInitialSwitchLocation = mFloorSwitch->GetComponentLocation();
}

// Called every frame
void AFloorSwitch::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFloorSwitch::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::White, FString::Printf(TEXT("Floor OverlapBegin")));
	if (!bCharacterOnSwitch)
		bCharacterOnSwitch = true;
	RaiseDoor();
	LowerFloorSwitch();
}

void AFloorSwitch::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::White, FString::Printf(TEXT("Floor OverlapEnd")));
	if (bCharacterOnSwitch)
		bCharacterOnSwitch = false;
	GetWorldTimerManager().SetTimer(mSwitchHandle, this, &AFloorSwitch::CloseDoor, mSwitchTime);
}

void AFloorSwitch::RaiseDoor()
{
	if (mDoorSound)
		UGameplayStatics::PlaySound2D(this, mDoorSound);
}

// ¹®Â¦
void AFloorSwitch::UpdateDoorLocation(float Z)
{
	FVector NewLocation = mInitialDoorLocation;
	NewLocation.Z += Z;
	mDoor->SetWorldLocation(NewLocation);
}

// ¹ßÆÇ
void AFloorSwitch::UpdateFloorSwitchLocation(float Z)
{
	FVector NewLocation = mInitialSwitchLocation;
	NewLocation.Z += Z;
	mFloorSwitch->SetWorldLocation(NewLocation);
}

void AFloorSwitch::CloseDoor()
{
	if (!bCharacterOnSwitch)
	{
		LowerDoor();
		RaiseFloorSwitch();
	}
}
