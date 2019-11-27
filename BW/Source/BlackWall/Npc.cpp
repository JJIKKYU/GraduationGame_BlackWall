// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC.h"
#include "BlackWallCharacter.h"
#include "Engine/Engine.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/BoxComponent.h"

// Sets default values
ANPC::ANPC()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Skeletal mesh componenet initialize
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMesh->SetupAttachment(GetRootComponent());

	// Meet Collision component initialize
	MeetCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("MeetCollision"));
	MeetCollision->SetupAttachment(GetRootComponent());
}


// Called when the game starts or when spawned
void ANPC::BeginPlay()
{
	Super::BeginPlay();
	
	MeetCollision->OnComponentBeginOverlap.AddDynamic(this, &ANPC::OnOverlapBegin);
	MeetCollision->OnComponentEndOverlap.AddDynamic(this, &ANPC::OnOverlapEnd);

	MeetCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	MeetCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	MeetCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	MeetCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
}

// Called every frame
void ANPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ANPC::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ANPC::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		ABlackWallCharacter* BWCharacter = Cast<ABlackWallCharacter>(OtherActor);
		if (BWCharacter)
		{
			GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::White, FString::Printf(TEXT("BWCharacter와 충돌되었습니다.")));
		}
	}
}

void ANPC::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}