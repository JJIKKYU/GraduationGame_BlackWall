// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BlackWallCharacter_Animinstance.generated.h"

/**
 * 
 */
UCLASS()
class BLACKWALL_API UBlackWallCharacter_Animinstance : public UAnimInstance
{
	GENERATED_BODY()
	

public: // function
	
	virtual void NativeInitializeAnimation() override;

	UFUNCTION(BlueprintCallable, Category = "AnimationProperties")
	void UpdateAnimationProperties();

public: // var
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float movementSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	bool bIsInAir;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	class APawn* Pawn;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	class ABlackWallCharacter* BlackWallCharacter;

	FORCEINLINE bool getIsInAir() { return bIsInAir; }
};
