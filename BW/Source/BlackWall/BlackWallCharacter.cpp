// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.
#include <EngineGlobals.h>
#include <Runtime/Engine/Classes/Engine/Engine.h>

#include "BlackWallCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
//////////////////////////////////////////////////////////////////////////
#include "Animation/AnimInstance.h" // UAnimInstance
#include "Runtime/Engine/Public/TimerManager.h" // set timers
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/Classes/Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "BWCharacterController.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Weapon.h"
#include "Enemy.h"
#include "Engine/Engine.h"
#include "Components/ChildActorComponent.h"
#include "save/mySaveGame.h"
#include "ItemStorage.h"


//////////////////////////////////////////////////////////////////////////
// ABlackWallCharacter

ABlackWallCharacter::ABlackWallCharacter()
	: BaseTurnRate(90.f), BaseLookUpRate(90.f)
	, bShiftDown(false), bLMBDown(false), bRMBDown(false), bSpaceDown(false), bIsInAir(false)

	// Movement and Status
	, MovementStatus(EMovementStatus::EMS_Normal), bIsCharacterForward(false), bIsCharacterRight(false)
	, bMovingForward(false), bMovingRight(false)
	, RunningSpeed(650.f)

	// Dash
	, bCanDash(true), bDashing(false), bDashStop(0.3f), mDashDistance(6000.f), mAirDashDistance(6000.f)
	, mDashCollDown(.5f), mDashUsingMP(15.f), DashStop(0.1f)
	, SprintingSpeed(1150.f), bCtrlKeyDown(false)

	// HP & MP
	, maxHP(100.f), hp(85.f), hpRecoveryRate(.1f)
	, maxMP(100.f), mp(50.f), mpRecoveryRate(.5f)
	, level(1), exp(0.f)

	// temporary var
	, bWeaponEquipped(false)

	// Attack
	, bAttacking(false), ComboCntA(0), ComboCntB(0), AttackMovementDistance(500.f), AirComboCntA(0)

	// Combat
	, bHasCombatTarget(false)

	// 보간
	, mInterpSpeed(15.f), bInterpToEnemy(false)
	
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 750.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller
	CameraBoom->ProbeChannel = ECC_WorldStatic;

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	/*
	ChildActor = FObjectInitializer().CreateDefaultSubobject<UChildActorComponent>(this, TEXT("Weapon"));
	ChildActor->SetChildActorClass(AWeapon::StaticClass());
	ChildActor->CreateChildActor();
	*/
}


void ABlackWallCharacter::BeginPlay()
{
	Super::BeginPlay();

	BWCharacterController = Cast<ABWCharacterController>(GetController());

	// Weapon Setup
	const USkeletalMeshSocket* SwordSocket = GetMesh()->GetSocketByName("sheath");
	if (SwordSocket)
	{
		if (EquippedWeapon)
		{
			EquippedWeapon->SetActorRelativeRotation(FRotator(0.f, 170.f, 0.f));
			SwordSocket->AttachActor(EquippedWeapon, this->GetMesh());
		}
	}


}

void ABlackWallCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (MovementStatus == EMovementStatus::EMS_Dead) return;

	// Auto Recovery HP, MP
	float DeltaMP = mpRecoveryRate * DeltaTime;
	float DeltaHP = hpRecoveryRate * DeltaTime;
	mp += DeltaMP;
	hp += DeltaHP;

	/* 레벨업 관리 */
	levelUp();

	bIsInAir = GetMovementComponent()->IsFalling();
	if (bIsInAir)
	{
		SetMovementStatus(EMovementStatus::EMS_Jump);
	}

	// 스프린팅 키를 눌렀을경우 (빠르게 달리기)
	if (bCtrlKeyDown)
		SetMovementStatus(EMovementStatus::EMS_Sprinting);
	else
		SetMovementStatus(EMovementStatus::EMS_Normal);

	/**
	if (bInterpToEnemy && CombatTarget)
	{
		FRotator LookAtYaw = GetLookAtRotationYaw(CombatTarget->GetActorLocation());
		FRotator InterpRotation = FMath::RInterpTo(GetActorRotation(), LookAtYaw, DeltaTime, mInterpSpeed);

		SetActorRotation(InterpRotation);
	}
	*/

	if (CombatTarget)
	{
		CombatTargetLocation = CombatTarget->GetActorLocation();
		if (BWCharacterController)
		{
			BWCharacterController->EnemyLocation = CombatTargetLocation;
		}
	}

	if (!EquippedWeapon)
	{
		if (WeaponStorage)
		{
			AItemStorage* Weapons = GetWorld()->SpawnActor<AItemStorage>(WeaponStorage);
			if (Weapons)
			{
				AWeapon* WeaponToEquip = GetWorld()->SpawnActor<AWeapon>(Weapons->WeaponMap["Weapon"]);
				// WeaponToEquip->Equip();
				EquippedWeapon = WeaponToEquip;
			}
		}
	}

	
}

void ABlackWallCharacter::SetMovementStatus(EMovementStatus status)
{
	MovementStatus = status;

	if (MovementStatus == EMovementStatus::EMS_Sprinting)
	{
		GetCharacterMovement()->MaxWalkSpeed = SprintingSpeed;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = RunningSpeed;
	}
}


//////////////////////////////////////////////////////////////////////////
// Input

void ABlackWallCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);

	
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	
	PlayerInputComponent->BindAxis("MoveForward", this, &ABlackWallCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ABlackWallCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ABlackWallCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ABlackWallCharacter::LookUpAtRate);

	// Attack ,Left Mouse Button & Game Pad Y Button Biding
	PlayerInputComponent->BindAction("LMB", IE_Pressed, this, &ABlackWallCharacter::LMBDown);
	PlayerInputComponent->BindAction("LMB", IE_Released, this, &ABlackWallCharacter::LMBUp);

	// Attack ComboB2 , Right Mouse button & Game Pad O Button Binding
	PlayerInputComponent->BindAction("RMB", IE_Pressed, this, &ABlackWallCharacter::RMBDown);
	PlayerInputComponent->BindAction("RMB", IE_Released, this, &ABlackWallCharacter::RMBUp);
	
	// Dash, Left Shift Key & Game Pad A Button Binding
	PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &ABlackWallCharacter::ShiftDown);
	PlayerInputComponent->BindAction("Dash", IE_Released, this, &ABlackWallCharacter::ShiftUp);

	// Sprinting, Left Ctrl Key & Game Pad 게임 패드는 아직 안 정함
	PlayerInputComponent->BindAction("Sprinting", IE_Pressed, this, &ABlackWallCharacter::CtrlKeyDown);
	PlayerInputComponent->BindAction("Sprinting", IE_Released, this, &ABlackWallCharacter::CtrlKeyUp);
}

void ABlackWallCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ABlackWallCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

bool ABlackWallCharacter::CanMove(float Value)
{
	if (BWCharacterController)
	{
		return (Value != 0.0f) && (!bAttacking);
	}
	return false;

}

void ABlackWallCharacter::MoveForward(float Value)
{
	// 움직이지 않으므로 false
	bMovingForward = false;

	if (CanMove(Value))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);

		// set Movement Status
		// SetMovementStatus(EMovementStatus::EMS_Moving);

		// set runningSpeed
		// GetCharacterMovement()->MaxWalkSpeed = RunningSpeed;

		// 움직이므로 true
		bMovingForward = true;

		/*
		set Character direction
		뒤로가면 0, 앞으로가면 1
		*/
		if (Value <= 0)
			bIsCharacterForward = false;
		else
			bIsCharacterForward = true;
	}
}

void ABlackWallCharacter::MoveRight(float Value)
{
	// 움직이지 않으므로 false
	bMovingRight = false;

	if (CanMove(Value))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);

		// set Movement Status
		// SetMovementStatus(EMovementStatus::EMS_Moving);

		// set runningSpeed
		// GetCharacterMovement()->MaxWalkSpeed = RunningSpeed;

		// 움직이므로 true
		bMovingRight = true;

		// set Character direction
		if (Value <= 0)
			bIsCharacterRight = false;
		else
			bIsCharacterRight = true;
	}
}

//////////////////////////////////////////////////////////////////////////
// Dash Ability
void ABlackWallCharacter::Dash()
{
	if (mp < mDashUsingMP) return;
	// If the character is alive and moving
	if (!bCanDash ||
		MovementStatus == EMovementStatus::EMS_Dead) return;
	UAnimInstance* Animation = GetMesh()->GetAnimInstance();
	if (!Animation && !UtilityMontage) return; // Define UtilityMontage in .h
	bDashing = true;

	SetMovementStatus(EMovementStatus::EMS_Dash);
	Animation->Montage_Play(UtilityMontage, 1.0f);
	UseMp(mDashUsingMP);

	// 공중에 있을 경우
	if (bIsInAir)
	{
		AirDash();
		//Animation->Montage_JumpToSection(FName("airDash"), UtilityMontage);
		//LaunchCharacter(FVector(GetActorForwardVector().X, GetActorForwardVector().Y, 0).GetSafeNormal() * mAirDashDistance, true, true);
		// GetCharacterMovement()->GravityScale = 0.5;
	}
	// 지상에 있을 경우
	else 
	{
		Animation->Montage_JumpToSection(FName("Dash"), UtilityMontage);
		LaunchCharacter(FVector(GetActorForwardVector().X, GetActorForwardVector().Y, 0).GetSafeNormal() * mDashDistance, true, true);
	}
	
	// ComboCnt Initialization
	ComboCntA = 0; ComboCntB = 0;
	
	GetCharacterMovement()->StopMovementImmediately();
	GetCharacterMovement()->BrakingFrictionFactor = .5f;
	
	bCanDash = false;
	
	if (bAttacking)
	{
		bAttacking = false; // if Attacking, Attack boolean initialize
	}
	GetWorldTimerManager().SetTimer(UnusedHandle, this, &ABlackWallCharacter::StopDashing, bDashStop, false);
}

void ABlackWallCharacter::AirDash()
{
	if (mp < mDashUsingMP) return;
	// If the character is alive and moving
	if (!bCanDash ||
		MovementStatus == EMovementStatus::EMS_Dead) return;
	UAnimInstance* Animation = GetMesh()->GetAnimInstance();
	if (!Animation && !UtilityMontage) return; // Define UtilityMontage in .h
	bDashing = true;

	if (bIsInAir)
	{
		Animation->Montage_JumpToSection(FName("airDash"), UtilityMontage);
	}
}

void ABlackWallCharacter::AirDashStart()
{
	GetCharacterMovement()->BrakingFrictionFactor = 0.f;
	LaunchCharacter(FVector(GetActorForwardVector().X, GetActorForwardVector().Y, 0).GetSafeNormal() * mAirDashDistance, true, true);

	GetCharacterMovement()->StopMovementImmediately();
	GetCharacterMovement()->BrakingFrictionFactor = .5f;

	bCanDash = false;

	if (bAttacking)
	{
		bAttacking = false; // if Attacking, Attack boolean initialize
	}
	GetWorldTimerManager().SetTimer(UnusedHandle, this, &ABlackWallCharacter::StopDashing, bDashStop, false);
}


void ABlackWallCharacter::ShiftDown()
{
	bShiftDown = true;
	if (MovementStatus == EMovementStatus::EMS_Dead) return;
	Dash();
}

void ABlackWallCharacter::StopDashing()
{
	GetCharacterMovement()->StopMovementImmediately();
	bDashing = false;
	GetCharacterMovement()->BrakingFrictionFactor = 2.f;
	GetWorldTimerManager().SetTimer(UnusedHandle, this, &ABlackWallCharacter::ResetDash, mDashCollDown, false);
	SetMovementStatus(EMovementStatus::EMS_Normal);
}

void ABlackWallCharacter::ResetDash()
{
	bCanDash = true;
}

void ABlackWallCharacter::PlayDashSound()
{
	if (DashSound)
		UGameplayStatics::PlaySound2D(this, DashSound);
}


//////////////////////////////////////////////////////////////////////////
// Atack
void ABlackWallCharacter::Attack()
{
	if (!bWeaponEquipped) return;
	if (bDashing || bAttacking || MovementStatus == EMovementStatus::EMS_Dash) return;
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (!AnimInstance || !AttackMontage) return;

	bAttacking = true;
	SetInterpToEnemy(true);
//	UE_LOG(LogTemp, Warning, TEXT("ATTACK"));
	
	if (ComboCntA == 0)
	{
		AnimInstance->Montage_Play(AttackMontage);
		AnimInstance->Montage_JumpToSection(FName("ComboA1"), AttackMontage);
		SetMovementStatus(EMovementStatus::EMS_Attack);
		GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::White, FString::Printf(TEXT("%d"), ComboCntA));
	}
	else if (ComboCntA == 1)
	{
		AnimInstance->Montage_Play(AttackMontage);
		AnimInstance->Montage_JumpToSection(FName("ComboA2"), AttackMontage);
		SetMovementStatus(EMovementStatus::EMS_Attack);
		GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::White, FString::Printf(TEXT("%d"), ComboCntA));
	}
	else if (ComboCntA == 2)
	{
		AnimInstance->Montage_Play(AttackMontage);
		AnimInstance->Montage_JumpToSection(FName("ComboA3"), AttackMontage);
		SetMovementStatus(EMovementStatus::EMS_Attack);
		GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::White, FString::Printf(TEXT("%d"), ComboCntA));
	}
	else if (ComboCntA == 3)
	{
		AnimInstance->Montage_Play(AttackMontage);
		AnimInstance->Montage_JumpToSection(FName("ComboA4"), AttackMontage);
		SetMovementStatus(EMovementStatus::EMS_Attack);
		GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::White, FString::Printf(TEXT("%d"), ComboCntA));
	}
	else if (ComboCntA == 4)
	{
		AnimInstance->Montage_Play(AttackMontage);
		AnimInstance->Montage_JumpToSection(FName("ComboA5"), AttackMontage);
		SetMovementStatus(EMovementStatus::EMS_Attack);
		GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::White, FString::Printf(TEXT("%d"), ComboCntA));
		ComboCntA = -1;
	}
}

void ABlackWallCharacter::AttackB()
{
	if (!bWeaponEquipped) return;
	if (bDashing || bAttacking || MovementStatus == EMovementStatus::EMS_Dash) return;
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (!AnimInstance || !AttackMontage) return;

	bAttacking = true;
	SetInterpToEnemy(true);

	if (ComboCntB == 0)
	{
		AnimInstance->Montage_Play(AttackMontage);
		AnimInstance->Montage_JumpToSection(FName("ComboB1"), AttackMontage);
		SetMovementStatus(EMovementStatus::EMS_Attack);
		GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::White, FString::Printf(TEXT("%d"), ComboCntB));
	}
	else if (ComboCntB == 1)
	{
		AnimInstance->Montage_Play(AttackMontage);
		AnimInstance->Montage_JumpToSection(FName("ComboB2"), AttackMontage);
		SetMovementStatus(EMovementStatus::EMS_Attack);
		GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::White, FString::Printf(TEXT("%d"), ComboCntB));
		//AnimInstance->Montage_SetNextSection(FName("ComboB1"), FName("ComboB2"), AttackMontage);
	}
	else if (ComboCntB == 2)
	{
		AnimInstance->Montage_Play(AttackMontage);
		AnimInstance->Montage_JumpToSection(FName("ComboB3"), AttackMontage);
		SetMovementStatus(EMovementStatus::EMS_Attack);
		GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::White, FString::Printf(TEXT("%d"), ComboCntB));
		//AnimInstance->Montage_SetNextSection(FName("ComboB2"), FName("ComboB3"), AttackMontage);
	}
	else if (ComboCntB == 3)
	{
		AnimInstance->Montage_Play(AttackMontage);
		AnimInstance->Montage_JumpToSection(FName("ComboB4"), AttackMontage);
		SetMovementStatus(EMovementStatus::EMS_Attack);
		GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::White, FString::Printf(TEXT("%d"), ComboCntB));
		//AnimInstance->Montage_SetNextSection(FName("ComboB3"), FName("ComboB4"), AttackMontage);
		ComboCntB = -1;
	}
}

void ABlackWallCharacter::AirAttack()
{
	GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::White, FString::Printf(TEXT("AirAttack Function Called")));

	if (!bWeaponEquipped) return;
	if (bDashing || bAttacking || MovementStatus == EMovementStatus::EMS_Dash) return;
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (!AnimInstance || !AirAttackMontage) return;

	bAttacking = true;
	SetInterpToEnemy(true);

	if (AirComboCntA == 0)
	{
		AnimInstance->Montage_Play(AirAttackMontage);
		AnimInstance->Montage_JumpToSection(FName("AirComboA1"), AirAttackMontage);
		SetMovementStatus(EMovementStatus::EMS_Attack);
		GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::White, FString::Printf(TEXT("%d"), AirComboCntA));
	}
	else if (AirComboCntA == 1)
	{
		AnimInstance->Montage_Play(AirAttackMontage);
		AnimInstance->Montage_JumpToSection(FName("AirComboA2"), AirAttackMontage);
		SetMovementStatus(EMovementStatus::EMS_Attack);
		GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::White, FString::Printf(TEXT("%d"), AirComboCntA));
	}
	else if (AirComboCntA == 2)
	{
		AnimInstance->Montage_Play(AirAttackMontage);
		AnimInstance->Montage_JumpToSection(FName("AirComboA3"), AirAttackMontage);
		SetMovementStatus(EMovementStatus::EMS_Attack);
		GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::White, FString::Printf(TEXT("%d"), AirComboCntA));
		AirComboCntA = -1;
	}
}

void ABlackWallCharacter::AttackEnd()
{
	// UE_LOG(LogTemp, Warning, TEXT("ATTACKEND"));
	SetMovementStatus(EMovementStatus::EMS_Normal);
	SetInterpToEnemy(false);
	bAttacking = false;
}

void ABlackWallCharacter::LMBDown()
{
	if (MovementStatus == EMovementStatus::EMS_Dead) return;
	if (bWeaponEquipped == false)
	{
		bWeaponEquipped = true;
		EquipWeapon();
		return;
	}
	//UE_LOG(LogTemp, Warning, TEXT("LMB DOWN"));
	bLMBDown = true;


	// 공중에 있을 경우에는 AirAttack 함수 호출
	if (bIsInAir)
	{
		AirAttack();
	}
	// 지상에 있을 경우에는 지상공격 Attack 함수 호출
	else
	{
		Attack();
		ComboCntB = 0;
		ComboCntA += 1;
	}
}

void ABlackWallCharacter::LMBUp()
{
	//UE_LOG(LogTemp, Warning, TEXT("LMB UP"));
	
	bLMBDown = false;
}

void ABlackWallCharacter::RMBDown()
{
	if (MovementStatus == EMovementStatus::EMS_Dead) return;
	if (bWeaponEquipped == false)
	{
		bWeaponEquipped = true;
		EquipWeapon();
		return;
	}
	bRMBDown = true;
	
	AttackB();
	ComboCntA = 0;
	ComboCntB += 1;
}

void ABlackWallCharacter::RMBUp()
{
	
	bRMBDown = false;
}


void ABlackWallCharacter::PlayAttackSound()
{
	if (mAttackSound[0])
	{
		UGameplayStatics::PlaySound2D(this, mAttackSound[0]);
	}
}

void ABlackWallCharacter::AttackMovement(float Amount)
{
	LaunchCharacter(FVector(GetActorForwardVector().X, GetActorForwardVector().Y, 0).GetSafeNormal() * Amount, true, true);
}

//////////////////////////////////////////////////////////////////////////
// Pickup Item

// Debuging 
void ABlackWallCharacter::ShowPickupLocations()
{
	for (auto Location : PickupLocations)
		UKismetSystemLibrary::DrawDebugSphere(this, Location, 25.f, 8, FLinearColor::Green, 10.f, 0.5f);
}

/**
* Function called if player has eaten potion
*/
void ABlackWallCharacter::IncrementHP(float Amount)
{
	if (hp + Amount >= maxHP) hp = maxHP;
	else hp += Amount;
}

void ABlackWallCharacter::IncrementMP(float Amount)
{
	if (mp + Amount >= maxMP) mp = maxMP;
	else mp += Amount;
}
//////////////////////////////////////////////////////////////////////////
// Sprinting 스프린팅/빠르게 달리기 기능

void ABlackWallCharacter::CtrlKeyDown()
{
	GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::White, FString::Printf(TEXT("CtrlKeyDown")));

	// 캐릭터가 움직일 때만 스프린팅이 가능함
	if (bMovingForward || bMovingRight)
	{
		bCtrlKeyDown = true;
	}
}

void ABlackWallCharacter::CtrlKeyUp()
{
	GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::White, FString::Printf(TEXT("CtrlKeyUp")));
	bCtrlKeyDown = false;
}



//////////////////////////////////////////////////////////////////////////
// Jump 점프기능

void ABlackWallCharacter::SpaceDown()
{
	if (MovementStatus == EMovementStatus::EMS_Dead) return;
	bSpaceDown = true;
	Jump();
}

void ABlackWallCharacter::SpaceUp()
{
	bSpaceDown = false;
}

void ABlackWallCharacter::Jump()
{
	if (MovementStatus != EMovementStatus::EMS_Dead)
	{
		ACharacter::Jump();
	}
	/*
	// If the character is alive and moving
	if (!bCanDash ||
		MovementStatus == EMovementStatus::EMS_Dead) return;
	UAnimInstance* Animation = GetMesh()->GetAnimInstance();
	if (!Animation && !JumpMontage) return; // Define UtilityMontage in .h
	// bJumping = true;
	setMovementStatus(EMovementStatus::EMS_Jump);
	Animation->Montage_Play(JumpMontage, 1.0f);
	Animation->Montage_JumpToSection(FName("Jump"), JumpMontage);

	// ComboCnt Initialization
	// ComboCntA = 0; ComboCntB = 0;
	*/
}

void ABlackWallCharacter::JumpEnd()
{	
	UE_LOG(LogTemp, Warning, TEXT("JumpEND()"));
	SetMovementStatus(EMovementStatus::EMS_Normal);

}



//////////////////////////////////////////////////////////////////////////
// Combat

void ABlackWallCharacter::UpdateCombatTarget()
{
	TArray<AActor*> OverlappingActors;
	GetOverlappingActors(OverlappingActors, EnemyFilter);

	if (OverlappingActors.Num() == 0)
	{
		if (BWCharacterController)
		{
			BWCharacterController->RemoveEnemyHealthBar();
		}
		return;
	}

	
	AEnemy* ClosestEnemy = Cast<AEnemy>(OverlappingActors[0]);
	if (ClosestEnemy)
	{
		FVector Location = GetActorLocation();
		float MinDistance = (ClosestEnemy->GetActorLocation() - Location).Size();

		for (auto Actor : OverlappingActors)
		{
			AEnemy* Enemy = Cast<AEnemy>(Actor);
			if (Enemy)
			{
				float DistanceToActor = (Enemy->GetActorLocation() - Location).Size();
				if (DistanceToActor < MinDistance)
				{
					MinDistance = DistanceToActor;
					ClosestEnemy = Enemy;
				}
			}
		}

		if (BWCharacterController)
		{
			BWCharacterController->DisplayEnemyHealthBar();
		}
		SetCombatTarget(ClosestEnemy);
		bHasCombatTarget = true;
	}
	
}

//////////////////////////////////////////////////////////////////////////
// Weapon

void ABlackWallCharacter::EquipWeapon()
{
	const USkeletalMeshSocket* SwordSocket = GetMesh()->GetSocketByName("Sword");
	if (SwordSocket)
	{
		EquippedWeapon->Equip();
		SwordSocket->AttachActor(EquippedWeapon, this->GetMesh());
		// Weapon Dissolve Effects
	}
	 if (OnEquipSound) UGameplayStatics::PlaySound2D(this, OnEquipSound);
}

void ABlackWallCharacter::UnEquipWeapon()
{
	const USkeletalMeshSocket* SwordSocket = GetMesh()->GetSocketByName("sheath");
	if (SwordSocket)
	{
		EquippedWeapon->UnEquip();
		SwordSocket->AttachActor(EquippedWeapon, this->GetMesh());
		bWeaponEquipped = false;
	}
	// if (OnEquipSound) UGameplayStatics::PlaySound2D(this, OnEquipSound);
}

//////////////////////////////////////////////////////////////////////////
// 보간

void ABlackWallCharacter::SetInterpToEnemy(bool Interp)
{
	bInterpToEnemy = Interp;
}

FRotator ABlackWallCharacter::GetLookAtRotationYaw(FVector Target)
{
	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Target);
	FRotator LookAtRotationYaw(0.f, LookAtRotation.Yaw, 0.f);
	return LookAtRotationYaw;
}

float ABlackWallCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (hp - DamageAmount <= 0.f)
	{
		hp = 0.f;
		Die();

		if (DamageCauser)
		{
			AEnemy* Enemy = Cast<AEnemy>(DamageCauser);
			if (Enemy)
				Enemy->bHasValidTarget = false;
		}
	}
	else
	{
		hp -= DamageAmount;
	}

	return DamageAmount;
}

void ABlackWallCharacter::Die()
{

}

//////////////////////////////////////////////////////////////////////////
// 레벨

void ABlackWallCharacter::levelUp()
{
	if (exp >= 100.f)
	{
		if (mlevelUpSound)
			UGameplayStatics::PlaySound2D(this, mlevelUpSound);
		level++;
		exp = 0.f;
	}
}

// 레벨이동
void ABlackWallCharacter::SwitchLevel(FName LevelName)
{
	UWorld* World = GetWorld();
	if (World)
	{
		FString CurrentLevel = World->GetMapName();
		FName CurrentLevelName(*CurrentLevel);
		if (CurrentLevelName != LevelName)
		{
			UGameplayStatics::OpenLevel(this, LevelName);
		}
	}
}

void ABlackWallCharacter::TestKey1()
{
	UmySaveGame* SaveGameInstance = Cast<UmySaveGame>(UGameplayStatics::CreateSaveGameObject(UmySaveGame::StaticClass()));

	if (SaveGameInstance)
	{
		if (EquippedWeapon)
		{
			SaveGameInstance->CharacterStats.WeaponName = EquippedWeapon->mName;
		}

		SaveGameInstance->CharacterStats.Rotation = GetActorRotation();
		SaveGameInstance->CharacterStats.Location = GetActorLocation();
	}

	UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveGameInstance->PlayerName, SaveGameInstance->UserIndex);
}

void ABlackWallCharacter::TestKey2()
{
	UmySaveGame* LoadGameInstance = Cast<UmySaveGame>(UGameplayStatics::CreateSaveGameObject(UmySaveGame::StaticClass()));
	if (LoadGameInstance)
	{
		LoadGameInstance = Cast<UmySaveGame>(UGameplayStatics::LoadGameFromSlot(LoadGameInstance->PlayerName, LoadGameInstance->UserIndex));

	}

	UmySaveGame* SaveGameInstance = Cast<UmySaveGame>(UGameplayStatics::CreateSaveGameObject(UmySaveGame::StaticClass()));
	if (SaveGameInstance)
	{
		if (EquippedWeapon)
		{
			SaveGameInstance->CharacterStats.WeaponName = EquippedWeapon->mName;
		}
		UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveGameInstance->PlayerName, SaveGameInstance->UserIndex);

		if (WeaponStorage)
		{
			AItemStorage* Weapons = GetWorld()->SpawnActor<AItemStorage>(WeaponStorage);
			if (Weapons)
			{
				FString WeaponName = SaveGameInstance->CharacterStats.WeaponName;

				if (WeaponName != TEXT(""))
				{
					AWeapon* WeaponToEquip = GetWorld()->SpawnActor<AWeapon>(Weapons->WeaponMap[WeaponName]);
					WeaponToEquip->Equip();
				}
			}
		}

		SetActorLocation(LoadGameInstance->CharacterStats.Location);
		SetActorRotation(LoadGameInstance->CharacterStats.Rotation);


	}

	
}