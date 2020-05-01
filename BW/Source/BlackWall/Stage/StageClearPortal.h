// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StageClearPortal.generated.h"

UCLASS()
class BLACKWALL_API AStageClearPortal : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AStageClearPortal();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Collider")
	class UBoxComponent* triggerBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
	class UUserWidget* tutorialWidget;

	class ABlackWallCharacter* BWCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster")
	FText monsterName;

	void CheckStageMonster(FText monsterTag);

	int monsterCount;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};
