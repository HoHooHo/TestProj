// Copyright Epic Games, Inc. All Rights Reserved.

#include "TestProjGameMode.h"
#include "TestProjCharacter.h"
#include "UObject/ConstructorHelpers.h"

ATestProjGameMode::ATestProjGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
