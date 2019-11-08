// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/BoxComponent.h"
#include "BlackWallCharacter.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/SkeletalMeshSocket.h"

AWeapon::AWeapon()
	// WeeponState
	: WeaponState(EWeaponState::EMS_Idle),

 	// Damage
	Damage(25.f)
{
	// Skeletal mesh component initialize
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMesh->SetupAttachment(GetRootComponent());

	// BoxComponent(Combatcollision) component initialize
	CombatCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("CombatCollision"));
	CombatCollision->SetupAttachment(GetRootComponent());

	OnDissolveMaterial = CreateDefaultSubobject<UMaterialInterface>(TEXT("OnDissolveMaterial"));
	
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
	//SkeletalMesh->CreateAndSetMaterialInstanceDynamicFromMaterial(0, OriginalMaterial);
	//SkeletalMesh->CreateAndSetMaterialInstanceDynamicFromMaterial(1, OnDissolveMaterial);
	SkeletalMesh->SetMaterial(0, OriginalMaterial);
	SkeletalMesh->SetMaterial(1, OnDissolveMaterial);
	//SkeletalMesh->OverrideMaterials.Add(OnDissolveMaterial);
	
	
	
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

void AWeapon::Equip(ABlackWallCharacter* BWCharacter)
{
	/*
	if (BWCharacter)
	{
		const USkeletalMeshSocket* SwordSocket = BWCharacter->GetMesh()->GetSocketByName("Sword");
		if (SwordSocket)
		{
			SwordSocket->AttachActor(this, BWCharacter->GetMesh());
			BWCharacter->SetEquippedWeapon(this);
		}
		if (OnEquipSound) UGameplayStatics::PlaySound2D(this, OnEquipSound);
	}
	*/
}

