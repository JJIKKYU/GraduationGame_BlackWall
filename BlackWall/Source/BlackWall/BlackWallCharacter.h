// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BlackWallCharacter.generated.h"

UENUM(BlueprintType)
enum class EMovementStatus : uint8
{
	EMS_Normal UMETA(DisplayName = "Normal"),
	EMS_Moving UMETA(DisplayName = "Moving"),
	EMS_Dash UMETA(DisplayName = "Dash"),
	EMS_Dead UMETA(DisplayName = "Dead")
};

UCLASS(config=Game)
class ABlackWallCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
public:
	ABlackWallCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;


	/**
	* MovementStatus
	* State of the character, expressed in Enum form
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Enums")
	EMovementStatus MovementStatus;
	// Set Movement Status adn running speed
	void setMovementStatus(EMovementStatus status);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Movement")
	float mRunningSpeed;


protected: // Player Input Interface

	/**
	* Attack
	* Left Mouse Button(LMB) Up and Down
	* Xbox Game Pad Y button
	*/
	bool bLMBDown;
	FORCEINLINE void LMBDown() { bLMBDown = true; }
	FORCEINLINE void LMBUp() { bLMBDown = false; }

	/**
	* Dash
	* Left Shift Button Up and Down
	* Xbox Game Pad A Button
	*/
	bool bShiftDown;
	void ShiftDown();
	FORCEINLINE void ShiftUp() { bShiftDown = false; }
	void Dash();
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anims | Utils")
	class UAnimMontage* UtilityMontage;

	UPROPERTY(EditAnywhere, Category = "Character | Movement")
	float DashDistance;
	UPROPERTY(EditAnywhere, Category = "Character | Movement")
	float DashCollDown;
	UPROPERTY(EditAnywhere, Category = "Character | Movement")
	float bDashStop;
	UPROPERTY()
	bool bCanDash;
	UPROPERTY()
	FTimerHandle UnusedHandle;
	UFUNCTION(BlueprintCallable)
	void StopDashing();
	UFUNCTION()
	void ResetDash();


	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

