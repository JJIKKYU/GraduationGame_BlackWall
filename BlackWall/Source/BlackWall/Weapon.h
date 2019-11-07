// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Weapon.generated.h"

UENUM(BlueprintType)
enum class EWeaponState : uint8
{
	EMS_Idle		UMETA(DisplayName = "Idle"),
	EMS_Equipped	UMETA(DisplayName = "Equipeed"),

	EMS_MAX			UMETA(DisplayName = "DefaultMax")
};

/**
 * 
 */
UCLASS()
class BLACKWALL_API AWeapon : public AItem
{
	GENERATED_BODY()


public:

	virtual void BeginPlay() override;

	AWeapon();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
	EWeaponState WeaponState;

///////////////////// Sound
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon | Sound")
	class USoundCue* OnEquipSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon | Sound")
	class USoundCue* SwingSound;

///////////////////// Skeletal Mesh
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SkeletalMesh")
	class USkeletalMeshComponent* SkeletalMesh;

///////////////////// Collsion
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon | Combat")
	class UBoxComponent* CombatCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon | Damage")
	float Damage;

	
	UFUNCTION(BlueprintCallable)
	void ActivateCollision();

	UFUNCTION(BlueprintCallable)
	void DeActivateCollision();
	


public: // FUNCTION

	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	
	UFUNCTION()
	virtual void CombatOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void CombatOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable)
	void Equip(class ABlackWallCharacter* BWCharacter);
	


	FORCEINLINE void SetWeaponState(EWeaponState State) { WeaponState = State; }
	FORCEINLINE EWeaponState GetWeaponState() { return WeaponState; }
};
