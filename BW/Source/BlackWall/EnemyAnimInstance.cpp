// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAnimInstance.h"
#include "Enemy.h"

void UEnemyAnimInstance::NativeInitializeAnimation()
{
	if (!mPawn)
	{
		mPawn = TryGetPawnOwner();

		if (mPawn)
		{
			mEnemy = Cast <AEnemy>(mPawn);
		}
	}
}

void UEnemyAnimInstance::UpdateAnimationProperties()
{
	if (!mPawn)
	{
		mPawn = TryGetPawnOwner();

		if (mPawn)
		{
			mEnemy = Cast<AEnemy>(mPawn);
		}
	}

	if (mPawn)
	{
		FVector Speed = mPawn->GetVelocity();
		FVector LateralSpeed = FVector(Speed.X, Speed.Y, 0.f);
		mMovementSpeed = LateralSpeed.Size();
	}
}
