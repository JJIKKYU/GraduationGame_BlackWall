// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

UENUM(BlueprintType)
enum class EEnemyMovementStatus : uint8
{
	EMS_Idle			UMETA(DisplayName = "Idle"),
	EMS_MoveToTarget	UMETA(DisplayName = "MoveToTarget"),
	EMS_Attacking		UMETA(DisplayName = "Attacking"),
	EMS_Death			UMETA(DisplayName = "Death"),
	EMS_MAX				UMETA(DisplayName = "DefaultMAX")
};

UCLASS()
class BLACKWALL_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	AEnemy();

	/**
	* Enemy MovementStatus
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	EEnemyMovementStatus EnemyMovementStatus;

	FORCEINLINE void SetEnemyMovementStatus(EEnemyMovementStatus Status) { EnemyMovementStatus = Status; }

	FORCEINLINE EEnemyMovementStatus GetEnemyMovementStatus() {	return EnemyMovementStatus; }

	/**
	* Enemy Status
	*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	float hp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	float maxHp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	float damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	float enemyExp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Status")
	bool bIsInAir;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Status")
	bool bIsDeath;

	/**
	* AI
	*/

	bool bHasValidTarget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	class USphereComponent* agroSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	class USphereComponent* combatSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	class AAIController* aiController;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	bool bOverlappingCombatSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	class ABlackWallCharacter* combatTarget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	class ABlackWallCharacter* BWCharacter;
	
	
	UFUNCTION()
	virtual void AgroSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void AgroSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	virtual void CombatSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void CombatSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	virtual void CombatOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void CombatOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


	/**
	* Collider
	*/

	UFUNCTION(BlueprintCallable)
	void ActivateCollision();

	UFUNCTION(BlueprintCallable)
	void DeactivateCollision();
	

	/**
	* Sound Cue
	*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds")
	class USoundCue* hitSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds")
	class USoundBase* deathSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds")
	class USoundBase* swingSound;



	/**
	* Attack
	*/

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	class UBoxComponent* combatCollision;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
	class UAnimMontage* combatMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	bool bAttacking;

	FTimerHandle attackTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float attackMinTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float attackMaxTime;

	UFUNCTION(BlueprintCallable)
	void MoveToTarget(class ABlackWallCharacter* Target);

	void Attack();

	UFUNCTION(BlueprintCallable)
	void AttackEnd();

	/**
	* Hit
	* Enemy가 맞았을경우
	*/

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Hit")
	class UAnimMontage* hitMontage;

	/**
	* Death
	*/
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	TSubclassOf<UDamageType> damageTypeClass;

	FTimerHandle deathTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float deathDelay;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	void Die(AActor* Causer);

	UFUNCTION(BlueprintCallable)
	void DeathEnd();

	UFUNCTION(BlueprintCallable)
	bool Alive();

	void Disappear();

	/**
	* Particle
	*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	class UParticleSystem* hitParticle;

	/*
	* Pickups 
	*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickups")
	TSubclassOf<class APickup> hpPickup;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickups")
	TSubclassOf<class APickup> mpPickup;

	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
