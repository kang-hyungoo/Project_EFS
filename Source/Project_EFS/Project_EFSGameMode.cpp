// Copyright Epic Games, Inc. All Rights Reserved.

#include "Project_EFSGameMode.h"
#include "Project_EFSCharacter.h"
#include "UObject/ConstructorHelpers.h"

AProject_EFSGameMode::AProject_EFSGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
