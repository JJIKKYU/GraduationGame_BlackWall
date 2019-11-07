// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

UENUM(BlueprintType)
enum class EEnemyMovementStatus : uint8
{
	EMS_Idle				UMETA(DisplayName = "Idle"),
	EMS_MoveToTarget		UMETA(DisplayName = "MoveToTarget"),
	EMS_Attacking			UMETA(DisplayName = "Attacking"),
	EMS_Death				UMETA(DisplayName = "Death"),

	EMS_MAX			UMETA(DisplayName = "DefaultMAX")
};

UCLASS()
class BLACKWALL_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	AEnemy();

	/**
	* MovementStatus
	* State of the Enemy, expressed in Enum form
	*/

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	EEnemyMovementStatus EnemyMovementStatus;

	FORCEINLINE void SetEnemyMovementStatus(EEnemyMovementStatus status) { EnemyMovementStatus = status; }

	/**
	* AI & Attack & Collision
	*/

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	class USphereComponent* mAgroSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	class USphereComponent* mCombatSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	class AAIController* mAIController;

	/**
	* Sound
	*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds")
	class USoundCue* mHitSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds")
	class USoundCue* mSwingSound;

public: // Enemy Stats

	UPROPERTY(EdiyAnywhere, BlueprintReadWrite, Category = "Stats")
	float mHP;

	UPROPERTY(EdiyAnywhere, BlueprintReadWrite, Category = "Stats")
	float mMaxHP;

	UPROPERTY(EdiyAnywhere, BlueprintReadWrite, Category = "Stats")
	float mDamage;




protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
