/*
* date : 2020.04.05
* description : 레벨 시퀀스 충돌 트리거 박스
*/


#include "CutSceneColliderBox.h"
#include "LevelSequence.h"
#include "LevelSequencePlayer.h"
#include "MovieScene.h"
#include "Components/BoxComponent.h"

#include "../BWCharacter/header/BlackWallCharacter.h"

// Sets default values
ACutSceneColliderBox::ACutSceneColliderBox()
	: bIsPlay(false)
{

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
void ACutSceneColliderBox::BeginPlay()
{
	Super::BeginPlay();
	
	triggerBox->OnComponentBeginOverlap.AddDynamic(this, &ACutSceneColliderBox::OnOverlapBegin);
	triggerBox->OnComponentEndOverlap.AddDynamic(this, &ACutSceneColliderBox::OnOverlapEnd);
}

// Called every frame
void ACutSceneColliderBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACutSceneColliderBox::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor) return;

	ABlackWallCharacter* BWCharacter = Cast<ABlackWallCharacter>(OtherActor);
	FMovieSceneSequencePlaybackSettings playbackSettings;
	if (BWCharacter)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("LEVEL SEQUENCE CHECK")));
		if (sequence && !bIsPlay)
		{
			ULevelSequencePlayer* LevelSequncePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), sequence, playbackSettings, levelSequenceActor);
			LevelSequncePlayer->Play();
			bIsPlay = true;
		}

	}
}

void ACutSceneColliderBox::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("LEVEL SEQUENCE UNCHECK")));

}

