// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnemyAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class BLACKWALL_API UEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movment")
	float mMovementSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movment")
	class APawn* mPawn;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movment")
	class AEnemy* mEnemy;

	virtual void NativeInitializeAnimation() override;

	UFUNCTION(BlueprintCallable, Category = "AnimationProperties")
	void UpdateAnimationProperties();
	
};
