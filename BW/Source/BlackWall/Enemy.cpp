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
#include "Engine/Engine.h"


AEnemy::AEnemy()
	:
	// Status
	hp(75.f), maxHp(100.f), damage(15.f), EnemyMovementStatus(EEnemyMovementStatus::EMS_Idle)
	, enemyExp(50.f)

	// AI
	, bHasValidTarget(false), bOverlappingCombatSphere(false)

	// Attack
	, attackMinTime(1.f), attackMaxTime(3.5f)

	// Death
	, deathDelay(4.f)

{
	
	PrimaryActorTick.bCanEverTick = true;

	// AgroSphere Initialized
	agroSphere = CreateDefaultSubobject<USphereComponent>(TEXT("mAgroSphere"));
	agroSphere->SetupAttachment(GetRootComponent());
	agroSphere->InitSphereRadius(600.f);

	// CombatSphere Initialized
	combatSphere = CreateDefaultSubobject<USphereComponent>(TEXT("mCombatSphere"));
	combatSphere->SetupAttachment(GetRootComponent());
	combatSphere->InitSphereRadius(75.f);

	combatCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("mCombatCollision"));
	combatCollision->SetupAttachment(GetMesh(), FName("weaponSocket"));

	combatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	combatCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	combatCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	combatCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);


	EnemyMovementStatus = EEnemyMovementStatus::EMS_Idle;
	
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	aiController = Cast<AAIController>(GetController());

	
	// AgroSphere Component Setting
	agroSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::AgroSphereOnOverlapBegin);
	agroSphere->OnComponentEndOverlap.AddDynamic(this, &AEnemy::AgroSphereOnOverlapEnd);

	// CombatSphere Component Setting
	combatSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::CombatSphereOnOverlapBegin);
	combatSphere->OnComponentEndOverlap.AddDynamic(this, &AEnemy::CombatSphereOnOverlapEnd);

	// Combat Coliision Setting
	combatCollision->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::CombatOnOverlapBegin);
	combatCollision->OnComponentEndOverlap.AddDynamic(this, &AEnemy::CombatOnOverlapEnd);
	

	// CombatCollision Initialized
	combatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	combatCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	combatCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	combatCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

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
			BWCharacter = BWCharacter;
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
			if (aiController)
				aiController->StopMovement();
		}
	}
}

void AEnemy::CombatSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		ABlackWallCharacter* BWCharacter = Cast<ABlackWallCharacter>(OtherActor);

		if (BWCharacter)
		{
			bHasValidTarget = true;
			BWCharacter->SetCombatTarget(this);
			BWCharacter->SetHasCombatTarget(true);
			BWCharacter->UpdateCombatTarget();

			combatTarget = BWCharacter;
			bOverlappingCombatSphere = true;

			float AttackTime = FMath::FRandRange(attackMinTime, attackMaxTime);
			GetWorldTimerManager().SetTimer(attackTimer, this, &AEnemy::Attack, AttackTime);
		}
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
			combatTarget = nullptr;

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
			GetWorldTimerManager().ClearTimer(attackTimer);
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
				UGameplayStatics::PlaySound2D(this, BWCharacter->mHitSound);
			}
			*/
			if (damageTypeClass)
			{
				UGameplayStatics::ApplyDamage(BWCharacter, damage, aiController, this, damageTypeClass);
			}
			
		}
	}
}

void AEnemy::CombatOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

void AEnemy::ActivateCollision()
{
	combatCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	if (swingSound)
	{
		UGameplayStatics::PlaySound2D(this, swingSound);
	}
}

void AEnemy::DeactivateCollision()
{
	combatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}


// If BWCharacter inside AgroSphere, Enemy move to BWCharacter

void AEnemy::MoveToTarget(ABlackWallCharacter* Target)
{
	SetEnemyMovementStatus(EEnemyMovementStatus::EMS_MoveToTarget);

	if (aiController)
	{
		FAIMoveRequest MoveRequest;
		MoveRequest.SetGoalActor(Target);
		MoveRequest.SetAcceptanceRadius(10.f);

		FNavPathSharedPtr NavPath;

		aiController->MoveTo(MoveRequest, &NavPath);
	}

	
	
}

void AEnemy::Attack()
{
	if (Alive() && bHasValidTarget)
	{
		if (aiController)
		{
			aiController->StopMovement();
			SetEnemyMovementStatus(EEnemyMovementStatus::EMS_Attacking);
		}

		if (!bAttacking)
		{
			bAttacking = true;
			UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
			if (AnimInstance && combatMontage)
			{
				AnimInstance->Montage_Play(combatMontage, 1.0f);
				AnimInstance->Montage_JumpToSection(FName("Attack_1"), combatMontage);
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
		float AttackTime = FMath::FRandRange(attackMinTime, attackMaxTime);
		GetWorldTimerManager().SetTimer(attackTimer, this, &AEnemy::Attack, AttackTime);
	}
}

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (hp - DamageAmount <= 0.0f)
	{
		hp = 0.0f;
		Die(DamageCauser);
	}
	else
	{
		hp -= DamageAmount;
	}
	return DamageAmount;
}

void AEnemy::Die(AActor* Causer)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && combatMontage)
	{
		AnimInstance->Montage_Play(combatMontage, 1.0f);
		AnimInstance->Montage_JumpToSection(FName("Death"), combatMontage);
	}
	SetEnemyMovementStatus(EEnemyMovementStatus::EMS_Death);
	if (deathSound) UGameplayStatics::PlaySound2D(this, deathSound);

	combatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	agroSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	combatSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	bAttacking = false;
	
	ABlackWallCharacter* BWCharacter = Cast<ABlackWallCharacter>(Causer);
	if (BWCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("CAST"));
		BWCharacter->UpdateCombatTarget();
	}

	if (BWCharacter)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::White, FString::Printf(TEXT("%f"), BWCharacter->getExp()));
		BWCharacter->setExp(BWCharacter->getExp() + enemyExp);
	}	
}

void AEnemy::DeathEnd()
{
	UE_LOG(LogTemp, Warning, TEXT("DEATHEND()"));
	GetMesh()->bPauseAnims = true;
	GetMesh()->bNoSkeletonUpdate = true;


	GetWorldTimerManager().SetTimer(deathTimer, this, &AEnemy::Disappear, deathDelay);
}

bool AEnemy::Alive()
{
	return (GetEnemyMovementStatus() != EEnemyMovementStatus::EMS_Death);
}

void AEnemy::Disappear()
{
	Destroy();
}
