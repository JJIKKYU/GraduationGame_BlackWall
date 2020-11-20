// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../header/Item.h"
#include "Weapon.generated.h"

UENUM(BlueprintType)
enum class EWeaponState : uint8
{
	EMS_Idle		UMETA(DisplayName = "Idle"),
	EMS_Equipped	UMETA(DisplayName = "Equipeed"),
	EMS_MAX			UMETA(DisplayName = "DefaultMax")
};

UENUM(BlueprintType)
enum class EAttackType : uint8
{
	EAT_Normal UMETA(DisplayName = "Normal"),
	EAT_Upper UMETA(DisplayName = "Upper"),
	EAT_Air UMETA(DisplayName = "Air")
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
	// Called every frame
	virtual void Tick(float DeltaTime) override;

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
	float damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon | Damage")
	float damageRef;

///////////////////// Mesh
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Material")
	class UMaterialInterface* DissolveMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Material")
	class UMaterialInterface* OriginalMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Material")
	UMaterialInstanceDynamic* DissolveMaterial_Dynamic;

	bool bMaterialChange;

	bool bEquipped;

	float appearenceValue;

	
	UFUNCTION(BlueprintCallable)
	void ActivateCollision();

	UFUNCTION(BlueprintCallable)
	void DeActivateCollision();

///////////////////// Combat

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	TSubclassOf<UDamageType> DamageTypeClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	AController* WeaponInstigator;
	

///////////////////// 레벨이동

	void SwitchLevel(FName LevelName);

	UPROPERTY(EditDefaultsOnly, Category = "SaveData")
	FString mName;

	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	
	UFUNCTION()
	virtual void CombatOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void CombatOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void Equip();
	void UnEquip();
	
	FORCEINLINE void SetWeaponState(EWeaponState State) { WeaponState = State; }
	FORCEINLINE EWeaponState GetWeaponState() { return WeaponState; }

	/**
	* Attack Type
	* 공격 타입
	* Upper Attack = Airebone Attack
	* Normal Attack
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Enums")
	EAttackType AttackType;

	FORCEINLINE void setAttackType(EAttackType type) { AttackType = type; }
	FORCEINLINE EAttackType getAttackType() { return AttackType; }

	float airBoneAttackJumpDistance;


	/*
	* Combo Cnt
	* 콤보카운트
	*/

	UPROPERTY(BlueprintReadWrite, Category = "Combo")
	int comboCnt;

	/*
	* Collision 관리
	*/

	bool bActivateCollision;

	class AEnemy* collisionEnemy;




};
