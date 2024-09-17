// Fill out your copyright notice in the Description page of Project Settings.


#include "HallowburgeSandboxGameModeBase.h"
#include "HallowburgePlayerCharacter.h"
#include "HallowburgePlayerController.h"

AHallowburgeSandboxGameModeBase::AHallowburgeSandboxGameModeBase()
{
	PrimaryActorTick.bCanEverTick = false;

	// Set default pawn class to your custom player character
	DefaultPawnClass = AHallowburgePlayerCharacter::StaticClass();

	// Set the player controller class to your custom player controller
	PlayerControllerClass = AHallowburgePlayerController::StaticClass();

}

void AHallowburgeSandboxGameModeBase::BeginPlay()
{
	Super::BeginPlay();

}
