// Fill out your copyright notice in the Description page of Project Settings.


#include "HallowburgePlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "InputMappingContext.h"
#include <Kismet/GameplayStatics.h>

void AHallowburgePlayerController::BeginPlay()
{
    Super::BeginPlay();

    // Ensure the character is valid and get the character movement component
    if (APawn* ControlledPawn = GetPawn())
    {
        PlayerCharacter = Cast<AGhostPlayerCharacter>(ControlledPawn);

        if (PlayerCharacter)
        {
            // Get the character's movement component and max walk speed
            if (PlayerCharacter->GetCharacterMovement())
            {
                MaxWalkSpeed = PlayerCharacter->GetCharacterMovement()->MaxWalkSpeed;
                PlayerCharacter->GetCharacterMovement()->AirControl = 0.8f;  // Set air control
            }
            // Get reference to the GameMode
            GameModeRef = Cast<AHallowburgeSandboxGameModeBase>(UGameplayStatics::GetGameMode(this));
        }
    }
}

void AHallowburgePlayerController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AHallowburgePlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

}

void AHallowburgePlayerController::LookUp(const float& InputValue)
{
	AddPitchInput(InputValue * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AHallowburgePlayerController::Turn(const float& InputValue)
{
	AddYawInput(InputValue * BaseLookRightRate * GetWorld()->GetDeltaSeconds());
}

void AHallowburgePlayerController::MoveRight(const float& InputValue)
{
    if (APawn* ControlledPawn = GetPawn())
    {
        if (InputValue != 0.0f)
        {
            const FRotator Rotation = ControlledPawn->GetControlRotation();
            const FRotator YawRotation(0, Rotation.Yaw, 0);

            const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
            ControlledPawn->AddMovementInput(Direction, InputValue);
        }
    }
}

void AHallowburgePlayerController::MoveForward(const float& InputValue)
{
        if (APawn* ControlledPawn = GetPawn())
    {
        if (InputValue != 0.0f)
        {
            const FRotator Rotation = ControlledPawn->GetControlRotation();
            const FRotator YawRotation(0, Rotation.Yaw, 0);

            const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
            ControlledPawn->AddMovementInput(Direction, InputValue);
        }
    }
}

void AHallowburgePlayerController::JumpFunction()
{
    if(PlayerCharacter)
    {
        PlayerCharacter->JumpFunction();
    }
}

void AHallowburgePlayerController::JumpFunctionEnd()
{
    if (PlayerCharacter)
    {
        PlayerCharacter->JumpFunctionEnd();
    }
}

void AHallowburgePlayerController::SprintStart()
{
    PlayerCharacter->GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
}

void AHallowburgePlayerController::SprintEnd()
{
    PlayerCharacter->GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
}

void AHallowburgePlayerController::PossessionAbilityCheck()
{
    if (PlayerCharacter)
    {
        PlayerCharacter->PossessionAbilityCheck();
    }
}

void AHallowburgePlayerController::Button1Action()
{
    if (PlayerCharacter)
    {
        PlayerCharacter->ActionButton1();
    }
}

void AHallowburgePlayerController::Button2Action()
{
    if (PlayerCharacter)
    {
        PlayerCharacter->ActionButton2();
    }
}

void AHallowburgePlayerController::Button1ActionEnd()
{
    if (PlayerCharacter)
    {
        PlayerCharacter->ActionButton1End();
    }
}

void AHallowburgePlayerController::Button2ActionEnd()
{
    if (PlayerCharacter)
    {
        PlayerCharacter->ActionButton2End();
    }
}
