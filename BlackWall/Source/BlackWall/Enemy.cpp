#include "Enemy.h"
#include "BlackWallCharacter.h"

#include "AIController.h"

#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Runtime/Engine/Public/TimerManager.h"
#include "BWCharacterController.h"
#include "Kismet/GameplayStatics.h"
#include "Animation/AnimInstance.h" // UAnimInstance


AEnemy::AEnemy()
	:
	// Status
	mHP(75.f), mMaxHP(100.f), mDamage(15.f), EnemyMovementStatus(EEnemyMovementStatus::EMS_Idle)

	// AI
	, bHasValidTarget(false), bOverlappingCombatSphere(false)

	// Attack
	, mAttackMinTime(1.f), mAttackMaxTime(3.5f)

	// Death
	, mDeathDelay(4.f)

{
	
	PrimaryActorTick.bCanEverTick = true;

	// AgroSphere Initialized
	mAgroSphere = CreateDefaultSubobject<USphereComponent>(TEXT("mAgroSphere"));
	mAgroSphere->SetupAttachment(GetRootComponent());
	mAgroSphere->InitSphereRadius(600.f);

	// CombatSphere Initialized
	mCombatSphere = CreateDefaultSubobject<USphereComponent>(TEXT("mCombatSphere"));
	mCombatSphere->SetupAttachment(GetRootComponent());
	mCombatSphere->InitSphereRadius(75.f);

	mCombatCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("mCombatCollision"));
	mCombatCollision->SetupAttachment(GetMesh(), FName("weaponSocket"));

	mCombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	mCombatCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	mCombatCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	mCombatCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);


	EnemyMovementStatus = EEnemyMovementStatus::EMS_Idle;
	
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	mAIController = Cast<AAIController>(GetController());

	
	// AgroSphere Component Setting
	mAgroSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::AgroSphereOnOverlapBegin);
	mAgroSphere->OnComponentEndOverlap.AddDynamic(this, &AEnemy::AgroSphereOnOverlapEnd);

	// CombatSphere Component Setting
	mCombatSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::CombatSphereOnOverlapBegin);
	mCombatSphere->OnComponentEndOverlap.AddDynamic(this, &AEnemy::CombatSphereOnOverlapEnd);

	// Combat Coliision Setting
	mCombatCollision->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::CombatOnOverlapBegin);
	mCombatCollision->OnComponentEndOverlap.AddDynamic(this, &AEnemy::CombatOnOverlapEnd);
	

	// CombatCollision Initialized
	mCombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	mCombatCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	mCombatCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	mCombatCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


void AEnemy::AgroSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		ABlackWallCharacter* BWCharacter = Cast<ABlackWallCharacter>(OtherActor);
		if (BWCharacter && Alive())
		{
			MoveToTarget(BWCharacter);
		}
	}
	
}

void AEnemy::AgroSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		ABlackWallCharacter* BWCharacter = Cast<ABlackWallCharacter>(OtherActor);
		if (BWCharacter)

		{
			bHasValidTarget = false;

			if (BWCharacter->GetCombatTarget() == this)
			{
				BWCharacter->SetCombatTarget(nullptr);
			}

			BWCharacter->SetHasCombatTarget(false);
			BWCharacter->UpdateCombatTarget();

			SetEnemyMovementStatus(EEnemyMovementStatus::EMS_Idle);
			if (mAIController)
				mAIController->StopMovement();
		}
	}
}

void AEnemy::CombatSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		ABlackWallCharacter* BWCharacter = Cast<ABlackWallCharacter>(OtherActor);

		if (!BWCharacter) return;

		bHasValidTarget = true;
		BWCharacter->SetCombatTarget(this);
		BWCharacter->SetHasCombatTarget(true);
		BWCharacter->UpdateCombatTarget();

		CombatTarget = BWCharacter;
		bOverlappingCombatSphere = true;

		float AttackTime = FMath::FRandRange(mAttackMinTime, mAttackMaxTime);
		GetWorldTimerManager().SetTimer(mAttackTimer, this, &AEnemy::Attack, AttackTime);
	}

}

void AEnemy::CombatSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && OtherComp)
	{
		ABlackWallCharacter* BWCharacter = Cast<ABlackWallCharacter>(OtherActor);
		if (BWCharacter)
		{
			bOverlappingCombatSphere = false;
			MoveToTarget(BWCharacter);
			CombatTarget = nullptr;

			if (BWCharacter->GetCombatTarget() == this)
			{
				BWCharacter->SetCombatTarget(nullptr);
				BWCharacter->SetHasCombatTarget(false);
				BWCharacter->UpdateCombatTarget();
			}
			if (BWCharacter->GetBWCharacterController())
			{
				USkeletalMeshComponent* BWCharacterMesh = Cast<USkeletalMeshComponent>(OtherComp);
				if (BWCharacterMesh)
				{
					BWCharacter->GetBWCharacterController()->RemoveEnemyHealthBar();
				}
			}
			// AttackDelay
			GetWorldTimerManager().ClearTimer(mAttackTimer);
		}
	}
}

void AEnemy::CombatOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		ABlackWallCharacter* BWCharacter = Cast<ABlackWallCharacter>(OtherActor);
		if (BWCharacter)
		{
			/*
			if (BWCharacter->GetHitParticle())
			{
				const USkeletalMeshSocket* HitEffectsSocket = GetMesh()->GetSocketByName("HitEffectsSocket");
				if (HitEffectsSocket)
				{
					FVector SocketLocation = HitEffectsSocket->GetSocketLocation(GetMesh());
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BWCharacter->GetHitParticle(), SocketLocation, FRotator(0.f), false);
				}
			}
			
			if (BWCharacter->IsValidHitSound())
			{
				UGameplayStatics::PlaySound2D(this, BWCharacter->mHitSoundd);
			}
			*/
			if (mDamageTypeClass)
			{
				UGameplayStatics::ApplyDamage(BWCharacter, mDamage, mAIController, this, mDamageTypeClass);
			}
			
		}
	}
}

void AEnemy::CombatOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

void AEnemy::ActivateCollision()
{
	mCombatCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	if (mSwingSound)
	{
		UGameplayStatics::PlaySound2D(this, mSwingSound);
	}
}

void AEnemy::DeactivateCollision()
{
	mCombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}


// If BWCharacter inside AgroSphere, Enemy move to BWCharacter

void AEnemy::MoveToTarget(ABlackWallCharacter* Target)
{
	SetEnemyMovementStatus(EEnemyMovementStatus::EMS_MoveToTarget);

	if (mAIController)
	{
		FAIMoveRequest MoveRequest;
		MoveRequest.SetGoalActor(Target);
		MoveRequest.SetAcceptanceRadius(10.f);

		FNavPathSharedPtr NavPath;

		mAIController->MoveTo(MoveRequest, &NavPath);
	}

	
	
}

void AEnemy::Attack()
{
	if (Alive() && bHasValidTarget)
	{
		if (mAIController)
		{
			mAIController->StopMovement();
			SetEnemyMovementStatus(EEnemyMovementStatus::EMS_Attacking);
		}

		if (!bAttacking)
		{
			bAttacking = true;
			UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
			if (AnimInstance && mCombatMontage)
			{
				AnimInstance->Montage_Play(mCombatMontage, 1.0f);
				AnimInstance->Montage_JumpToSection(FName("Attack_1"), mCombatMontage);
			}
		}
	}
	
}

void AEnemy::AttackEnd()
{
	bAttacking = false;
	// 여전히 CombatSphere안에 있을경우
	if (bOverlappingCombatSphere)
	{
		float AttackTime = FMath::FRandRange(mAttackMinTime, mAttackMaxTime);
		GetWorldTimerManager().SetTimer(mAttackTimer, this, &AEnemy::Attack, AttackTime);
	}
}

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (mHP - DamageAmount <= 0.0f)
	{
		mHP = 0.0f;
		Die(DamageCauser);
	}
	else
	{
		mHP -= DamageAmount;
	}
	return DamageAmount;
}

void AEnemy::Die(AActor* Causer)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && mCombatMontage)
	{
		AnimInstance->Montage_Play(mCombatMontage, 1.0f);
		AnimInstance->Montage_JumpToSection(FName("Death"), mCombatMontage);
	}
	SetEnemyMovementStatus(EEnemyMovementStatus::EMS_Death);

	mCombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	mAgroSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	mCombatSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	bAttacking = false;

	ABlackWallCharacter* BWCharacter = Cast<ABlackWallCharacter>(Causer);
	if (BWCharacter)
		BWCharacter->UpdateCombatTarget();
}

void AEnemy::DeathEnd()
{
	UE_LOG(LogTemp, Warning, TEXT("DEATHEND()"));
	GetMesh()->bPauseAnims = true;
	GetMesh()->bNoSkeletonUpdate = true;

	GetWorldTimerManager().SetTimer(mDeathTimer, this, &AEnemy::Disappear, mDeathDelay);
}

bool AEnemy::Alive()
{
	return (GetEnemyMovementStatus() != EEnemyMovementStatus::EMS_Death);
}

void AEnemy::Disappear()
{
	Destroy();
}
