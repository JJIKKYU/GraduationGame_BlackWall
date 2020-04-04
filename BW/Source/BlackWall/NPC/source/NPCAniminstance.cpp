// Fill out your copyright notice in the Description page of Project Settings.


#include "../header/NPCAniminstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../header/NPC.h"

void UNPCAniminstance::NativeInitializeAnimation()
{
	// If there is no Pawn
	if (!Pawn) Pawn = TryGetPawnOwner();
	// If there is Pawn
	if (Pawn)
	{
		NpcCharacter = Cast<ANPC>(Pawn);
	}
}

void UNPCAniminstance::UpdateAnimationProperties()
{
	// If there is no Pawn
	if (!Pawn) Pawn = TryGetPawnOwner();

	// If there is Pawn
	if (Pawn)
	{
		FVector speed = Pawn->GetVelocity();
		FVector LateralSpeed = FVector(speed.X, speed.Y, 0.f);
		movementSpeed = LateralSpeed.Size();

		// If there is no BlackWallCharacter
		if (!NpcCharacter) NpcCharacter = Cast<ANPC>(Pawn);
	}
}

