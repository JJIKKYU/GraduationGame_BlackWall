// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "../header/BlackWallGameMode.h"
#include "../header/BlackWallCharacter.h"
#include "UObject/ConstructorHelpers.h"

ABlackWallGameMode::ABlackWallGameMode()
{
	// set default pawn class to our Blueprinted character
	ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/3_Character/BlackWallCharacter_BP.BlackWallCharacter_BP_C"));
	if (PlayerPawnBPClass.Class)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	questTitle = "1234";
}
