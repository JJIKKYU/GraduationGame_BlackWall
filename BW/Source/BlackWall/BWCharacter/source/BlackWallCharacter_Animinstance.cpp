// Fill out your copyright notice in the Description page of Project Settings.


#include "../header/BlackWallCharacter_Animinstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../header/BlackWallCharacter.h"

void UBlackWallCharacter_Animinstance::NativeInitializeAnimation()
{
	// If there is no Pawn
	if (!Pawn) Pawn = TryGetPawnOwner();
	// If there is Pawn
	if (Pawn)
	{
		BlackWallCharacter = Cast<ABlackWallCharacter>(Pawn);
	}
}

void UBlackWallCharacter_Animinstance::UpdateAnimationProperties()
{
	// If there is no Pawn
	if (!Pawn) Pawn = TryGetPawnOwner();

	// If there is Pawn
	if (Pawn)
	{
		FVector speed = Pawn->GetVelocity();
		FVector LateralSpeed = FVector(speed.X, speed.Y, 0.f);
		movementSpeed = LateralSpeed.Size();

		bIsInAir = Pawn->GetMovementComponent()->IsFalling();

		// If there is no BlackWallCharacter
		if (!BlackWallCharacter) BlackWallCharacter = Cast<ABlackWallCharacter>(Pawn);
	}
}
