// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

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
#include "Engine/Classes/Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"


//////////////////////////////////////////////////////////////////////////
// ABlackWallCharacter

ABlackWallCharacter::ABlackWallCharacter()
	: BaseTurnRate(90.f), BaseLookUpRate(90.f)
	, bShiftDown(false), bLMBDown(false)

	// Movement and Status
	, MovementStatus(EMovementStatus::EMS_Normal), bIsCharacterForward(true), bIsCharacterRight(false)
	, mRunningSpeed(850.f)
	, bCanDash(true), bDashing(false), bDashStop(0.15f), mDashDistance(6000.f), mDashCollDown(.5f), mDashUsingMP(15.f)

	// HP & MP
	, mMaxHP(100.f), mHP(85.f), mHPrecoveryRate(.1f)
	, mMaxMP(100.f), mMP(50.f), mMPrecoveryRate(.5f)

	// temporary var
	, bWeaponEquipped(false)

	// Attack
	, bAttacking(false)
	
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
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
}

void ABlackWallCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (MovementStatus == EMovementStatus::EMS_Dead) return;

	// Auto Recovery HP, MP
	float DeltaMP = mMPrecoveryRate * DeltaTime;
	float DeltaHP = mHPrecoveryRate * DeltaTime;
	mMP += DeltaMP;
	mHP += DeltaHP;
}

void ABlackWallCharacter::setMovementStatus(EMovementStatus status)
{
	MovementStatus = status;
}


//////////////////////////////////////////////////////////////////////////
// Input


void ABlackWallCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);

	/*
	// Jump is not use this game
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	*/

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
	
	// Dash, Left Shift Key & Game Pad A Button Binding
	PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &ABlackWallCharacter::ShiftDown);
	PlayerInputComponent->BindAction("Dash", IE_Released, this, &ABlackWallCharacter::ShiftUp);
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

void ABlackWallCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);

		// set Movement Status
		//setMovementStatus(EMovementStatus::EMS_Moving);

		// set runningSpeed
		GetCharacterMovement()->MaxWalkSpeed = mRunningSpeed;

		// set Character direction
		if (Value <= 0)
			bIsCharacterForward = false;
		else
			bIsCharacterForward = true;
	}
}

void ABlackWallCharacter::MoveRight(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);

		// set Movement Status
		//setMovementStatus(EMovementStatus::EMS_Moving);

		// set runningSpeed
		GetCharacterMovement()->MaxWalkSpeed = mRunningSpeed;

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
	if (mMP < mDashUsingMP) return;
	// If the character is alive and moving
	if (!bCanDash ||
		MovementStatus == EMovementStatus::EMS_Dead) return;
	UAnimInstance* Animation = GetMesh()->GetAnimInstance();
	if (!Animation && !UtilityMontage) return; // Define UtilityMontage in .h
	bDashing = true;
	setMovementStatus(EMovementStatus::EMS_Dash);
	Animation->Montage_Play(UtilityMontage, 1.0f);
	Animation->Montage_JumpToSection(FName("Dash"), UtilityMontage);
	UseMp(mDashUsingMP);
	
	GetCharacterMovement()->StopMovementImmediately();
	GetCharacterMovement()->BrakingFrictionFactor = 0.f;
	LaunchCharacter(FVector(GetActorForwardVector().X, GetActorForwardVector().Y, 0).GetSafeNormal() * mDashDistance,true, true);
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
//	GetCharacterMovement()->StopMovementImmediately();
	bDashing = false;
	GetCharacterMovement()->BrakingFrictionFactor = 2.f;
	GetWorldTimerManager().SetTimer(UnusedHandle, this, &ABlackWallCharacter::ResetDash, mDashCollDown, false);
	setMovementStatus(EMovementStatus::EMS_Normal);
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
	if (bDashing || bAttacking || MovementStatus == EMovementStatus::EMS_Dash) return;
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (!AnimInstance || !AttackMontage) return;
	bAttacking = true;
	setMovementStatus(EMovementStatus::EMS_Attack);
	UE_LOG(LogTemp, Warning, TEXT("ATTACK"));
	AnimInstance->Montage_Play(AttackMontage);
}

void ABlackWallCharacter::AttackEnd()
{
	UE_LOG(LogTemp, Warning, TEXT("ATTACKEND"));
	setMovementStatus(EMovementStatus::EMS_Normal);
	bAttacking = false;
}

void ABlackWallCharacter::LMBDown()
{
	if (MovementStatus == EMovementStatus::EMS_Dead) return;
	UE_LOG(LogTemp, Warning, TEXT("LMB DOWN"));
	bLMBDown = true;
	Attack();
}

void ABlackWallCharacter::LMBUp()
{
	UE_LOG(LogTemp, Warning, TEXT("LMB UP"));
	bLMBDown = false;
}

void ABlackWallCharacter::PlayAttackSound()
{
	if (AttackSound[0])
	{
		UGameplayStatics::PlaySound2D(this, AttackSound[0]);
	}
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
	if (mHP + Amount >= mMaxHP) mHP = mMaxHP;
	else mHP += Amount;
}

void ABlackWallCharacter::IncrementMP(float Amount)
{
	if (mMP + Amount >= mMaxMP) mMP = mMaxMP;
	else mMP += Amount;
}
