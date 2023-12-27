// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameMode/CPP_ShootingGameMode.h"
#include "GameMode/CPP_ShootingGameCharacter.h"
#include "UObject/ConstructorHelpers.h"

ACPP_ShootingGameMode::ACPP_ShootingGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
