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
	EMS_Attack UMETA(DisplayName = "Attack"),
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


protected: // Player Stats
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character | Stats")
	float mMaxHP;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character | Stats")
	float mHP;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character | Stats")
	float mHPrecoveryRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character | Stats")
	float mMaxMP;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character | Stats")
	float mMP;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character | Stats")
	float mMPrecoveryRate;

	FORCEINLINE void UseMp(float mp) { mMP -= mp; }



protected: // Player Input Interface

///////////////////

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds | DashSound")
	class USoundCue* DashSound;

	UFUNCTION(BlueprintCallable)
	void PlayDashSound();

	UPROPERTY(EditAnywhere, Category = "Character | Movement")
	float mDashDistance;
	
	UPROPERTY(EditAnywhere, Category = "Character | Movement")
	float mDashCollDown;
	
	UPROPERTY(EditAnywhere, Category = "Character | Movement")
	float bDashStop;

	UPROPERTY(EditAnywhere, Category = "Character | Movement")
	float mDashUsingMP;
	
	UPROPERTY()
	bool bCanDash;

	bool bDashing;
	
	UPROPERTY()
	FTimerHandle UnusedHandle;
	
	UFUNCTION(BlueprintCallable)
	void StopDashing();
	
	UFUNCTION()
	void ResetDash();

///////////////////
	
	/**
	* Attack
	* Left Mouse Button Up and Down
	* Xbox Game Pad Y Button
	*/

	bool bLMBDown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Weapon")
	bool bWeaponEquipped;

	void LMBDown();
	void LMBUp();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Anims | Attack")
	bool bAttacking;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anims | Attacak")
	class UAnimMontage* AttackMontage;

	void Attack();

	UFUNCTION(BlueprintCallable)
	void AttackEnd();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds | AttackSound")
	TArray<class USoundCue*> AttackSound;

	UFUNCTION(BlueprintCallable)
	void PlayAttackSound();


///////////////////


	/** Called for forwards/backward input */
	void MoveForward(float Value);
	bool bIsCharacterForward;

	/** Called for side to side input */
	void MoveRight(float Value);
	bool bIsCharacterRight;

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

public: // Pickup Item

	TArray<FVector> PickupLocations;

	UFUNCTION(BlueprintCallable)
	void ShowPickupLocations();

	UFUNCTION(BlueprintCallable)
	void IncrementHP(float Amount);

	UFUNCTION(BlueprintCallable)
	void IncrementMP(float Amount);

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

