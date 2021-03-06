// Fill out your copyright notice in the Description page of Project Settings.


#include "../header/Weapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/BoxComponent.h"
#include "../../../BWCharacter/header/BlackWallCharacter.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "../../../Enemy/header/Enemy.h"
#include "Engine/Engine.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../../../GameInstnace/MainGameInstance.h"

AWeapon::AWeapon()
	// WeeponState
	: WeaponState(EWeaponState::EMS_Idle)

 	// Damage
	, damage(10.f), damageRef(damage)

	// Material
	, bMaterialChange(false), appearenceValue(0.f), bEquipped(false)

	, airBoneAttackJumpDistance(850.f)

	, comboCnt(0), bActivateCollision(false)
{
	// Skeletal mesh component initialize
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMesh->SetupAttachment(GetRootComponent());

	// BoxComponent(Combatcollision) component initialize
	CombatCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("CombatCollision"));
	CombatCollision->SetupAttachment(GetRootComponent());

	DissolveMaterial = CreateDefaultSubobject<UMaterialInterface>(TEXT("DissolveMaterial"));
	
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	// If CombatCollision collided, function call CombatOnOverlapBegin
	// else if CombatCollsion collided out, function call CombatOnOverlapEnd
	CombatCollision->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::CombatOnOverlapBegin);
	CombatCollision->OnComponentEndOverlap.AddDynamic(this, &AWeapon::CombatOnOverlapEnd);

	CombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CombatCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	CombatCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	CombatCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	// Weapon Material Initialize
	SkeletalMesh->CreateAndSetMaterialInstanceDynamicFromMaterial(0, OriginalMaterial);	
	DissolveMaterial_Dynamic = Mesh->CreateAndSetMaterialInstanceDynamic(0);
	DissolveMaterial_Dynamic = UMaterialInstanceDynamic::Create(DissolveMaterial, this);
}

void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bMaterialChange)
	{
		appearenceValue += (DeltaTime * 1.5f);
		UE_LOG(LogTemp, Log, TEXT("%f"), appearenceValue);
		DissolveMaterial_Dynamic->SetScalarParameterValue(TEXT("Appearence"), appearenceValue);

		if (appearenceValue >= 1.f)
		{
			Equip();
			bMaterialChange = false;
			return;
		}
	}

	damage = rand() % int(damageRef) + 5;
}

void AWeapon::ActivateCollision()
{
	CombatCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	bActivateCollision = true;
	// GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::White, FString::Printf(TEXT("bActivateCollision = true;")));
}

void AWeapon::DeActivateCollision()
{
	CombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	bActivateCollision = false;
	collisionEnemy = nullptr;
	// GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::White, FString::Printf(TEXT("bActivateCollision = false;")));
}

void AWeapon::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void AWeapon::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}

void AWeapon::CombatOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// if (bActivateCollision == false) return;
	// if (collisionEnemy == Cast<AEnemy>(OtherActor)) return;

	if (OtherActor)
	{
		AEnemy* Enemy = Cast<AEnemy>(OtherActor);
		// collisionEnemy = Enemy;
		

		if (Enemy)
		{
			UMainGameInstance* mainGameInstance = Cast<UMainGameInstance>(GetGameInstance());
			if (mainGameInstance)
			{
				mainGameInstance->SetComboCnt(mainGameInstance->GetComboCnt() + 1);
				// GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::White, FString::Printf(TEXT("ComboCnt = %d"), mainGameInstance->GetComboCnt()));
			}
			

			if (EAttackType::EAT_Normal == AttackType)
			{

			}
			else if (EAttackType::EAT_Upper == AttackType)
			{
				Enemy->LaunchCharacter(FVector(0, 0, 1).GetSafeNormal() * airBoneAttackJumpDistance, true, true);
			}
			else if (EAttackType::EAT_Air == AttackType)
			{
				Enemy->GetCharacterMovement()->StopMovementImmediately();
				Enemy->LaunchCharacter(FVector(0, 0, 1).GetSafeNormal() * 250, true, true);

// 				Enemy->GetCharacterMovement()->GravityScale = 0.2f;
			}
			
			// GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::White, FString::Printf(TEXT("OverlapBegin_Weapon")));
			
			if (Enemy->hitParticle)
			{
				const USkeletalMeshSocket* WeaponSocket = SkeletalMesh->GetSocketByName("WeaponSocket");
				if (WeaponSocket)
				{
					FVector SocketLocation = WeaponSocket->GetSocketLocation(SkeletalMesh);
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Enemy->hitParticle, SocketLocation, FRotator(FMath::Rand()%180), false);
				}
			}
			if (Enemy->hitSound)
			{
				UGameplayStatics::PlaySound2D(this, Enemy->hitSound);
			}
			
			//death
			if (DamageTypeClass)
			{
				UGameplayStatics::ApplyDamage(Enemy, damage, WeaponInstigator, this, DamageTypeClass);
			}
		}
	}
}

void AWeapon::CombatOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::White, FString::Printf(TEXT("OverlapEnd_Weapon")));
}

void AWeapon::Equip()
{
	bMaterialChange = true;
	appearenceValue = 0.f;
	SkeletalMesh->SetMaterial(0, DissolveMaterial_Dynamic);
}

void AWeapon::UnEquip()
{
	bMaterialChange = true;
	appearenceValue = 0.f;	
}

////// �����̵�

void AWeapon::SwitchLevel(FName LevelName)
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

