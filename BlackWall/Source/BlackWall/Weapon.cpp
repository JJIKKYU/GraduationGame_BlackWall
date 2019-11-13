// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/BoxComponent.h"
#include "BlackWallCharacter.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Materials/MaterialInstanceDynamic.h"

AWeapon::AWeapon()
	// WeeponState
	: WeaponState(EWeaponState::EMS_Idle)

 	// Damage
	, Damage(25.f)

	// Material
	, bMaterialChange(false), appearenceValue(0.f), bEquipped(false)
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
}

void AWeapon::ActivateCollision()
{
	CombatCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AWeapon::DeActivateCollision()
{
	CombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
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
}

void AWeapon::CombatOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
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


