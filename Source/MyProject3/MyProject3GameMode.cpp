// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "MyProject3.h"
#include "MyProject3GameMode.h"
#include "MyProject3Character.h"

AMyProject3GameMode::AMyProject3GameMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Characters/man/man_Character"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
