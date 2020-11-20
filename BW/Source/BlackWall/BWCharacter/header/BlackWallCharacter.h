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
	EMS_Sprinting UMETA(DisplayName = "Sprinting"),
	EMS_Dash UMETA(DisplayName = "Dash"),
	EMS_Attack UMETA(DisplayName = "Attack"),
	EMS_AirAttack UMETA(DisplayName = "AirAttack"),
	EMS_Jump UMETA(DisplayName = "Jump"),
	EMS_Dead UMETA(DisplayName = "Dead")
};



UENUM(BlueprintType)
enum class EButtonType : uint8
{
	EBT_RMB UMETA(DisplayName = "RMB"),
	EBT_LMB UMETA(DisplayName = "LMB")
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

	// true일 경우 움직일 수 있고, false일때 움직일 수 없음.
	bool bCanMove;

	void setCanMove(bool flag) { bCanMove = flag; }

	// Set Movement Status adn running speed
	void SetMovementStatus(EMovementStatus status);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Movement")
	float RunningSpeed;

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
	float exp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character | Stats")
	int level;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character | Stats")
	float maxHP;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Stats")
	float hp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character | Stats")
	float hpRecoveryRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character | Stats")
	float maxMP;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character | Stats")
	float mp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character | Stats")
	float mpRecoveryRate;

	void LevelUp();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Effects")
	class UParticleSystem* levelUpParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds | Stats")
	class USoundBase* mlevelUpSound;

	FORCEINLINE void UseMp(float mp)
	{
		if (this->mp - mp <= 0)
			this->mp = 0;
		else
			this->mp -= mp;
	}

	FORCEINLINE int getLevel() { return level; }
	FORCEINLINE void setLevel(int level) { this->level = level; }
	
	FORCEINLINE float getExp() { return exp; }
	FORCEINLINE void setExp(float exp) { this->exp = exp; }

///////////////////

	/**
	* CameraArmControl
	* 대쉬 및 스프린팅 할 때 카메라 거리 조절
	*
	*/

	/** 스프린팅 및 대쉬, 공격할 때 armLength를 유동적으로 컨트롤 */
	void ArmLengthControl(const float DeltaTime);

///////////////////

	/**
	* Sprint
	* 
	* 
	*/

	bool bCtrlKeyDown;

	

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character | Sprint")
	float SprintingSpeed;

	/** Pressed down to enable sprinting */
	void CtrlKeyDown();

	/** Released to stop sprinting */
	void CtrlKeyUp();

	


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

	UFUNCTION(BlueprintCallable)
	void AirDash();

	UFUNCTION(BlueprintCallable)
	void AirDashStart();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anims | Utils")
	class UAnimMontage* UtilityMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds | DashSound")
	class USoundCue* DashSound;

	UFUNCTION(BlueprintCallable)
	void PlayDashSound();

	UPROPERTY(EditAnywhere, Category = "Character | Movement")
	float dashDistance;

	UPROPERTY(EditAnywhere, Category = "Character | Movement")
	float airDashDistance;

	UPROPERTY(EditAnywhere, Category = "Character | Movement")
	float airBoneAttackJumpDistance;
	
	UPROPERTY(EditAnywhere, Category = "Character | Movement")
	float dashCoolDown;
	
	UPROPERTY(EditAnywhere, Category = "Character | Movement")
	float dashStop;

	UPROPERTY(EditAnywhere, Category = "Character | Movement")
	float dashUsingMP;
	
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
	bool bAltDown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Weapon")
	bool bWeaponEquipped;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attack | Attack")
	bool bAttacking;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attack | AirAttack")
	bool bAirAttacking;

	// 공중공격시 공격 버튼 클릭 유무
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack | AirAttack")
	bool bPressedAttackButtonWhenAirAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack | Attack")
	bool bPressedAttackButtonWhenAttack;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack | Attack")
	class UAnimMontage* AttackMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack | AirAttack")
	class UAnimMontage* AirAttackMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack | AirAttack")
	class UAnimMontage* AirBoneAttackMontage;

	int comboCntA;
	int comboCntB;	
	int airComboCntA;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack | Attack")
	float attackMovementDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack | Effects")
	class UParticleSystem* mHitParticle;


	/**
	* Air Dash Attack Var
	*/
	bool bCanAirDashAttack;
	float AirDashStop;
	float AirDashAttackCoolDown;

	// 점프공격시 잠깐 떠있도록 velocityValue값을 조절
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack | AirAttack")
	FVector velocityValue;

	// 점프공격시 잠깐 떠있도록 gravityScaleValue값을 조절
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack | AirAttack")
	float gravityScaleValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack | AirAttack")
	float gravityScaleDefaultValue;

	// AirDashAttack 쿨타임 조절 Handle
	UPROPERTY()
	FTimerHandle AirDashAttackUnusedHandle;

	/**
	* Attack Sound
	*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds | AttackSound")
	TArray<class USoundCue*> mAttackSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds | AttackSound")
	TArray<class USoundCue*> mHitSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds | AttackSound")
	class USoundBase* mHitSoundd;

	

	// Function

	void LMBDown();
	FORCEINLINE void LMBUp() { bLMBDown = false; }
	void RMBDown();
	FORCEINLINE void RMBUp() { bRMBDown = false; }
	void altDown();
	FORCEINLINE void altUp() { bAltDown = false; }

	/**
	* Attack Function
	*/

	void Attack();
	void AttackB();

	// 지상 공격시 LMBDown할 경우, bPressedAttackButtonWhenAttack change true or false
	UFUNCTION(BlueprintCallable)
	void ComboInputChecking();

	UFUNCTION(BlueprintCallable)
	void ComboInputCheckingAttackB();

	UFUNCTION(BlueprintCallable)
	void AttackEnd();

	UFUNCTION(BlueprintCallable)
	void LastAttackChecking();

	UFUNCTION(BlueprintCallable)
	void AttackMovement(float Amount);

	/**
	* Air Dash Attack Function
	*/

	void AirDashAttack();
	void StopAirDashAttacking();
	void AirDashAttackReset();

	//

	UFUNCTION(BlueprintCallable)
	void PlayAttackSound();

	FORCEINLINE UParticleSystem* GetHitParticle() { return mHitParticle; }

	FORCEINLINE TArray<class USoundCue*> GetHitSound() { return mHitSound; }

	FORCEINLINE bool IsValidHitSound()
	{
		if (mHitSound[0]) return true;
		return false;
	}

	/**
	* Air Attack Function
	*/

	// Velocity 및 Gravity 등 AirAttack 관리
	void AirAttackManager();
	void AirAttack();
	void AirBoneAttack(bool bIsInAir);

	// AirBoneAttack 애니메이션 진행 중, Jump 타이밍 조절을 위한 함수 (엔진에서 Notify로 호출)
	UFUNCTION(BlueprintCallable)
	void AirBoneAttackJumping();

	UFUNCTION(BlueprintCallable)
	 void AirBoneAttackJumping_AttackB();

	UFUNCTION(BlueprintCallable)
	void AirBoneAttackJumpingReverse();

	void StopAirBoneAttackJumping();

	// 공중 공격시 LMBDown할 경우, bPressedAttackButtonWhenAirAttack change true or false
	UFUNCTION(BlueprintCallable)
	void AirComboInputChecking();

	/**
	* 공격이 어떤 유형인지
	*/
	EMovementStatus getMovementStatus();
	


///////////////////

	/**
	/* Jump
	*/

	bool bSpaceDown;
	bool bIsInAir;

	void SpaceDown();
	FORCEINLINE void SpaceUp() { bSpaceDown = false; }

	virtual void Jump();

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
	class AEnemy* TargetingEnemy;

	FRotator TargetingEnemyRotator;
	bool bIsLock;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	TSubclassOf<AActor> EnemyFilter;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Combat")
	FVector CombatTargetLocation;

	UFUNCTION(BlueprintCallable)
	void TargetingCameraLockOn();

	void UpdateCombatTarget();

	FORCEINLINE AEnemy* GetCombatTarget() {	return CombatTarget; }

	FORCEINLINE void SetHasCombatTarget(bool HasTarget) { bHasCombatTarget = HasTarget;	}

	FORCEINLINE void SetCombatTarget(AEnemy* Enemy) { CombatTarget = Enemy;	}


///////////////////

	/*
	* 보간
	*/

	float interpSpeed;
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


	/**
	Called for forwards/backward input
	*/
	void MoveForward(float Value);
	// bIsCharacterForward = Forward Move : true, Backward move : false
	bool bIsCharacterForward;
	// bMovingForward = Forward Moving : true, Not Moving : false
	bool bMovingForward;

	/**
	Called for side to side input
	*/
	void MoveRight(float Value);
	bool bIsCharacterRight;
	bool bMovingRight;

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

