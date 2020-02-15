#pragma once

#include "Engine/Engine.h"

class UtilFunc
{
	UtilFunc();

	void debugMessage(const char* text)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::White, FString::Printf(text));
	}
};