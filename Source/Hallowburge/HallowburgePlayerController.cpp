// Fill out your copyright notice in the Description page of Project Settings.


#include "HallowburgePlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h" // Add this line
#include <Kismet/GameplayStatics.h>

void AHallowburgePlayerController::BeginPlay()
{
    Super::BeginPlay();

    // Ensure the character is valid and get the character movement component
    if (APawn* ControlledPawn = GetPawn())
    {
        PlayerCharacter = Cast<AHallowburgePlayerCharacter>(ControlledPawn);

        if (PlayerCharacter)
        {
            // Get the character's movement component and max walk speed
            if (PlayerCharacter->GetCharacterMovement())
            {
                MaxWalkSpeed = PlayerCharacter->GetCharacterMovement()->MaxWalkSpeed;
                PlayerCharacter->GetCharacterMovement()->AirControl = 0.8f;  // Set air control
            }
            // Set up the enhanced input mapping context
            if (UEnhancedInputLocalPlayerSubsystem* InputSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
            {
                if (!InputMapping.IsNull())
                {
                    InputSystem->AddMappingContext(InputMapping.LoadSynchronous(), 0);
                }
            }
            // Get reference to the GameMode
            GameModeRef = Cast<AHallowburgeSandboxGameModeBase>(UGameplayStatics::GetGameMode(this));
        }
    }
}

void AHallowburgePlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
    {
        // input actions
        EnhancedInputComponent->BindAction(LookUpAction, ETriggerEvent::Triggered, this, &AHallowburgePlayerController::LookUp);
        EnhancedInputComponent->BindAction(TurnAction, ETriggerEvent::Triggered, this, &AHallowburgePlayerController::Turn);
        EnhancedInputComponent->BindAction(MoveRightAction, ETriggerEvent::Triggered, this, &AHallowburgePlayerController::MoveRight);
        EnhancedInputComponent->BindAction(MoveForwardAction, ETriggerEvent::Triggered, this, &AHallowburgePlayerController::MoveForward);

        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AHallowburgePlayerController::JumpFunction);
        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AHallowburgePlayerController::JumpFunctionEnded);
        EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Triggered, this, &AHallowburgePlayerController::CrouchStart);
        EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Completed, this, &AHallowburgePlayerController::CrouchEnd);

        EnhancedInputComponent->BindAction(PossessionAction, ETriggerEvent::Triggered, this, &AHallowburgePlayerController::PossessionFunction);
        EnhancedInputComponent->BindAction(Ability1Action, ETriggerEvent::Triggered, this, &AHallowburgePlayerController::Button1Action);
        EnhancedInputComponent->BindAction(Ability2Action, ETriggerEvent::Triggered, this, &AHallowburgePlayerController::Button2Action);
    }
}

void AHallowburgePlayerController::LookUp(const FInputActionValue& InputAction)
{
	const float LookUpValue = InputAction.Get<float>();
	AddPitchInput(LookUpValue * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AHallowburgePlayerController::Turn(const FInputActionValue& InputAction)
{
	const float TurnValue = InputAction.Get<float>();
	AddYawInput(TurnValue * BaseLookRightRate * GetWorld()->GetDeltaSeconds());
}

void AHallowburgePlayerController::MoveRight(const FInputActionValue& InputAction)
{
    const float AxisValue = InputAction.Get<float>();
    if (APawn* ControlledPawn = GetPawn())
    {
        if (AxisValue != 0.0f)
        {
            const FRotator Rotation = ControlledPawn->GetControlRotation();
            const FRotator YawRotation(0, Rotation.Yaw, 0);

            const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
            ControlledPawn->AddMovementInput(Direction, AxisValue);
        }
    }
}

void AHallowburgePlayerController::MoveForward(const FInputActionValue& InputAction)
{
    const float AxisValue = InputAction.Get<float>();
        if (APawn* ControlledPawn = GetPawn())
    {
        if (AxisValue != 0.0f)
        {
            const FRotator Rotation = ControlledPawn->GetControlRotation();
            const FRotator YawRotation(0, Rotation.Yaw, 0);

            const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
            ControlledPawn->AddMovementInput(Direction, AxisValue);
        }
    }
}

void AHallowburgePlayerController::JumpFunction()
{
    if (PlayerCharacter)
    {
        PlayerCharacter->Jump();
    }
}

void AHallowburgePlayerController::JumpFunctionEnded()
{
    if (PlayerCharacter)
    {
        PlayerCharacter->StopJumping();
    }
}

void AHallowburgePlayerController::CrouchStart()
{
    PlayerCharacter->Crouch();
}

void AHallowburgePlayerController::CrouchEnd()
{
    PlayerCharacter->UnCrouch();
}

void AHallowburgePlayerController::PossessionFunction()
{
    // Possession/dash
}

void AHallowburgePlayerController::Button1Action()
{
    // Punch ability
}

void AHallowburgePlayerController::Button2Action()
{

}
