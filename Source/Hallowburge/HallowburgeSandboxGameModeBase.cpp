// Fill out your copyright notice in the Description page of Project Settings.


#include "HallowburgeSandboxGameModeBase.h"
#include "PossessableCharacter.h"
#include "HallowburgePlayerController.h"

AHallowburgeSandboxGameModeBase::AHallowburgeSandboxGameModeBase()
{
	PrimaryActorTick.bCanEverTick = false;

	// Set default pawn class to your custom player character
	DefaultPawnClass = APossessableCharacter::StaticClass();

	// Set the player controller class to your custom player controller
	PlayerControllerClass = AHallowburgePlayerController::StaticClass();

}

void AHallowburgeSandboxGameModeBase::BeginPlay()
{
	Super::BeginPlay();

}
