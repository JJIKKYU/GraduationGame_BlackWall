// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CutSceneColliderBox.generated.h"

UCLASS()
class BLACKWALL_API ACutSceneColliderBox : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACutSceneColliderBox();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// 플레이하고자 하는 레벨 시퀀스 포인터
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sequence")
	class ULevelSequence* sequence;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Checkpoint | Collider")
	class UBoxComponent* triggerBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sequence")
	class ALevelSequenceActor* levelSequenceActor;

	bool bIsPlay;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
