// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAnimInstance.h"
#include "Enemy.h"
#include "GameFramework/CharacterMovementComponent.h"

void UEnemyAnimInstance::NativeInitializeAnimation()
{
	if (!pawn)
	{
		pawn = TryGetPawnOwner();

		if (pawn)
		{
			enemy = Cast <AEnemy>(pawn);
		}
	}
}

void UEnemyAnimInstance::UpdateAnimationProperties()
{
	if (!pawn)
	{
		pawn = TryGetPawnOwner();

		if (pawn)
		{
			enemy = Cast<AEnemy>(pawn);
		}
	}

	if (pawn)
	{
		bIsInAir = pawn->GetMovementComponent()->IsFalling();

		FVector Speed = pawn->GetVelocity();
		FVector LateralSpeed = FVector(Speed.X, Speed.Y, 0.f);
		movementSpeed = LateralSpeed.Size();
	}
}