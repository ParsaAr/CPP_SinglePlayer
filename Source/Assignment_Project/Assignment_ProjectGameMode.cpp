// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "Assignment_ProjectGameMode.h"
#include "Assignment_ProjectCharacter.h"
#include "UObject/ConstructorHelpers.h"

AAssignment_ProjectGameMode::AAssignment_ProjectGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
