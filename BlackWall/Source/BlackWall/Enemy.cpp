#include "Enemy.h"
#include "BlackWallCharacter.h"

#include "AIController.h"

#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"

AEnemy::AEnemy()
	:
	// Status
	mHP(75.f), mMaxHP(100.f), mDamage(15.f)

	// AI
	,bHasValidTarget(false)

{
	
	PrimaryActorTick.bCanEverTick = true;

	// AgroSphere Initialized
	mAgroSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AgroSphere"));
	mAgroSphere->SetupAttachment(GetRootComponent());
	mAgroSphere->InitSphereRadius(600.f);

	// CombatSphere Initialized
	mCombatSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CombatSphere"));
	mCombatSphere->SetupAttachment(GetRootComponent());
	mCombatSphere->InitSphereRadius(75.f);

	mCombatCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("CombatCollision"));
	mCombatCollision->SetupAttachment(GetRootComponent());

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
	mCombatSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::AgroSphereOnOverlapBegin);
	mCombatSphere->OnComponentEndOverlap.AddDynamic(this, &AEnemy::AgroSphereOnOverlapEnd);
	

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
	if (!OtherActor) return;
	ABlackWallCharacter* BWCharacter = Cast<ABlackWallCharacter>(OtherActor);
	if (!BWCharacter) return;

	MoveToTarget(BWCharacter);
}

void AEnemy::AgroSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!OtherActor) return;
	ABlackWallCharacter* BWCharacter = Cast<ABlackWallCharacter>(OtherActor);
	if (!BWCharacter) return;

	///

	bHasValidTarget = false;

	BWCharacter->SetHasCombatTarget(false);
	BWCharacter->UpdateCombatTarget();
	
	SetEnemyMovementStatus(EEnemyMovementStatus::EMS_Idle);
	if (mAIController) mAIController->StopMovement();
}

void AEnemy::CombatSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor) return;
	ABlackWallCharacter* BWCharacter = Cast<ABlackWallCharacter>(OtherActor);
	if (!BWCharacter) return;

	bHasValidTarget = true;


}

void AEnemy::CombatSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && OtherComp) return;
	ABlackWallCharacter* BWCharacter = Cast<ABlackWallCharacter>(OtherActor);
	if (!BWCharacter) return;
	
}

void AEnemy::CombatOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void AEnemy::CombatOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}


// If BWCharacter inside AgroSphere, Enemy move to BWCharacter

void AEnemy::MoveToTarget(ABlackWallCharacter* Target)
{
	
	if (!mAIController) return;

	SetEnemyMovementStatus(EEnemyMovementStatus::EMS_MoveToTarget);

	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalActor(Target);
	MoveRequest.SetAcceptanceRadius(10.f);

	FNavPathSharedPtr NavPath;

	mAIController->MoveTo(MoveRequest, &NavPath);
	
}
