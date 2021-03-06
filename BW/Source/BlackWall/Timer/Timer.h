// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Timer.generated.h"

UCLASS()
class BLACKWALL_API ATimer : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATimer();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timer")
	float timerCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timer")
	bool bEndOfTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timer")
	bool bIsStart;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timer")
	bool bIsClear;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
