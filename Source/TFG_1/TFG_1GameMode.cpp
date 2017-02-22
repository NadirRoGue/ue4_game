// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "TFG_1.h"
#include "TFG_1GameMode.h"
#include "TFG_1HUD.h"
#include "TFG_1Character.h"

ATFG_1GameMode::ATFG_1GameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/GameMode/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = ATFG_1HUD::StaticClass();
}
