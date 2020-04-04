// Fill out your copyright notice in the Description page of Project Settings.


#include "../header/Pickup.h"
#include "../../../BWCharacter/header/BlackWallCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Engine/Classes/Sound/SoundCue.h"

APickup::APickup()
	: IncrementHPAmount(25.f), IncrementMPAmount(50.f)
{

}

void APickup::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	if (!OtherActor) return;
	// 
	ABlackWallCharacter* BWCharacter = Cast<ABlackWallCharacter>(OtherActor);
	if (!BWCharacter) return;

	OnPickupBP(BWCharacter);
	BWCharacter->PickupLocations.Add(GetActorLocation());

	if (OverlapSound)
	{
		UGameplayStatics::PlaySound2D(this, OverlapSound);
	}

	Destroy();
}

void APickup::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}
