// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TutorialCheckpoint.generated.h"

UCLASS()
class BLACKWALL_API ATutorialCheckpoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATutorialCheckpoint();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Checkpoint | Collider")
	class UBoxComponent* mTriggerBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Checkpoint | Widget")
	class UUserWidget* mTutorialWidget;

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
