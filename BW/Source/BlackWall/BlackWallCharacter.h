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
	EMS_Jump UMETA(DisplayName = "Jump"),
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

	bool CanMove(float Value);

public: ///////////////////
	// Controller


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Controller")
	class ABWCharacterController* BWCharacterController;

	FORCEINLINE ABWCharacterController* GetBWCharacterController() { return BWCharacterController; }

///////////////////
	// Weapon Equip

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds | Weapon")
	class USoundCue* OnEquipSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	class AWeapon* EquippedWeapon;
	
	UPROPERTY(VisibleDefaultsOnly, Category = "Weapon")
	UChildActorComponent* ChildActor;

	UPROPERTY(EditDefaultsOnly, Category = "SaveData")
	TSubclassOf<class AItemStorage> WeaponStorage;

	UFUNCTION(Blueprintcallable)
	void EquipWeapon();

	UFUNCTION(Blueprintcallable)
	void UnEquipWeapon();


///////////////////
	// Player Stats

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character | Stats")
	float mExp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character | Stats")
	int mLevel;
	
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

	void levelUp();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds | Stats")
	class USoundBase* mlevelUpSound;

	FORCEINLINE void UseMp(float mp) { mMP -= mp; }

	FORCEINLINE int getLevel() { return mLevel; }
	FORCEINLINE void setLevel(int level) { mLevel = level; }
	
	FORCEINLINE float getExp() { return mExp; }
	FORCEINLINE void setExp(float exp) { mExp = exp; }



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

public:
	
	/**
	* Attack
	* Left Mouse Button Up and Down
	* Xbox Game Pad Y Button
	*/

	// Var

	bool bLMBDown;
	bool bRMBDown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Weapon")
	bool bWeaponEquipped;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Anims | Attack")
	bool bAttacking;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anims | Attack")
	class UAnimMontage* AttackMontage;

	int ComboCntA;
	int ComboCntB;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Attack")
	float AttackMovementDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Attack")
	class UParticleSystem* mHitParticle;

	// Function

	void LMBDown();
	void LMBUp();

	void RMBDown();
	void RMBUp();


	void Attack();
	void AttackB();

	UFUNCTION(BlueprintCallable)
	void AttackEnd();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds | AttackSound")
	TArray<class USoundCue*> mAttackSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds | AttackSound")
	TArray<class USoundCue*> mHitSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds | AttackSound")
	class USoundBase* mHitSoundd;

	UFUNCTION(BlueprintCallable)
	void PlayAttackSound();

	UFUNCTION(BlueprintCallable)
	void AttackMovement(float Amount);

	FORCEINLINE UParticleSystem* GetHitParticle() { return mHitParticle; }

	FORCEINLINE TArray<class USoundCue*> GetHitSound() { return mHitSound; }

	FORCEINLINE bool IsValidHitSound()
	{
		if (mHitSound[0]) return true;
		return false;
	}


///////////////////

	/**
	/* Jump
	*/

	bool bSpaceDown;

	void SpaceDown();
	void SpaceUp();

	void Jump();

	UFUNCTION(BlueprintCallable)
	void JumpEnd();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anims | Jump")
	class UAnimMontage* JumpMontage;


///////////////////

	/**
	/* Combat
	*/

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	bool bHasCombatTarget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	class AEnemy* CombatTarget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	TSubclassOf<AActor> EnemyFilter;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Combat")
	FVector CombatTargetLocation;


	void UpdateCombatTarget();

	FORCEINLINE AEnemy* GetCombatTarget() {	return CombatTarget; }

	FORCEINLINE void SetHasCombatTarget(bool HasTarget) { bHasCombatTarget = HasTarget;	}

	FORCEINLINE void SetCombatTarget(AEnemy* Enemy) { CombatTarget = Enemy;	}


///////////////////

	/*
	* 보간
	*/

	float mInterpSpeed;
	bool bInterpToEnemy;
	void SetInterpToEnemy(bool Interp);

	FRotator GetLookAtRotationYaw(FVector Target);


///////////////////

	/*
	* Damage
	*/

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	void Die();

/////////////////// 레벨이동

	void SwitchLevel(FName LevelName);

	UFUNCTION(BlueprintCallable)
	void TestKey1();

	UFUNCTION(BlueprintCallable)
	void TestKey2();

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
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

