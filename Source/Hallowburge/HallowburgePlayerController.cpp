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

void AHallowburgePlayerController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // Handle Jetpack
    switch (JetpackState)
    {
    case EJetpackState::Active:
        ConsumeJetpackFuel(DeltaTime);
        break;

    case EJetpackState::Regenerating:
        RefuelJetpack(DeltaTime);
        break;
    case EJetpackState::Empty:
        JetpackDeactivate();
        break;
    case EJetpackState::Idle:
    default:
        // Do nothing when idle
        break;
    }
    UE_LOG(LogTemp, Display, TEXT("Current amount of fuel: %f"), CurrentJetpackFuel);
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
        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AHallowburgePlayerController::JumpFunctionEnd);
        EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Triggered, this, &AHallowburgePlayerController::SprintStart);
        EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &AHallowburgePlayerController::SprintEnd);

        EnhancedInputComponent->BindAction(PossessionAction, ETriggerEvent::Triggered, this, &AHallowburgePlayerController::PossessionAbilityCheck);
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
        if (PlayerCharacter->GetCharacterMovement()->IsFalling())
        {
            switch (JetpackState)
            {
            case EJetpackState::Empty:
                // Jetpack is empty, fallback to normal jump behavior
                PlayerCharacter->Jump();
                break;

            case EJetpackState::Regenerating:
                // Jetpack is inactive when refueling, so cannot do anything but stick with the regular jump logic
                PlayerCharacter->Jump();
                break;
            default:
                // Jetpack is ready to use!
                JetpackActive();
                break;
            }
        }
        else
        {
            // Grounded, so perform a normal jump
            PlayerCharacter->Jump();
        }
    }
}

void AHallowburgePlayerController::JumpFunctionEnd()
{
    if (PlayerCharacter)
    {
        if (JetpackState == EJetpackState::Active)
        {
            JetpackDeactivate();
        }
        else
        {
            // Stop the regular jump if jetpack isn't active
            PlayerCharacter->StopJumping();
        }
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

void AHallowburgePlayerController::JetpackActive()
{
    if (PlayerCharacter && PlayerCharacter->GetCharacterMovement() && CurrentJetpackFuel > 0)
    {
        JetpackState = EJetpackState::Active; // Set to active to consume fuel
        PlayerCharacter->GetCharacterMovement()->GravityScale = 0.0f; // Disable gravity
        FVector Velocity = PlayerCharacter->GetVelocity();
        FVector NewVelocity = FVector(Velocity.X, Velocity.Y, 250.0f);
        PlayerCharacter->GetCharacterMovement()->Velocity = NewVelocity;
    }
}

void AHallowburgePlayerController::JetpackDeactivate()
{
    if (PlayerCharacter && PlayerCharacter->GetCharacterMovement())
    {
        // Return gravity to normal
        PlayerCharacter->GetCharacterMovement()->GravityScale = 1.0f;

        // Switch back to Idle state when jetpack is deactivated
        JetpackState = EJetpackState::Regenerating; // was originally idle
    }
}

void AHallowburgePlayerController::ConsumeJetpackFuel(float DeltaTime)
{
    // Decrease fuel based on DeltaTime (e.g., 20 units of fuel per second)
    CurrentJetpackFuel -= 20.0f * DeltaTime;

    // If fuel runs out, deactivate jetpack
    if (CurrentJetpackFuel <= 0)
    {
        CurrentJetpackFuel = 0;
        JetpackState = EJetpackState::Empty;
    }
}

void AHallowburgePlayerController::RefuelJetpack(float DeltaTime)
{
    // Regenerate fuel over time
    if (CurrentJetpackFuel < MaxJetpackFuel)
    {
        CurrentJetpackFuel += RefuelJetpackRate * DeltaTime;
        CurrentJetpackFuel = FMath::Clamp(CurrentJetpackFuel, 0.0f, MaxJetpackFuel);

        // Stop regenerating once fully refueled
        if (CurrentJetpackFuel >= MaxJetpackFuel)
        {
            JetpackState = EJetpackState::Idle;
        }
    }
}

void AHallowburgePlayerController::PossessionAbilityCheck()
{
    switch (PlayerCharacter->PlayerPawn)
    {
    case EPlayerPawn::Ghost: // The ghost will dash, and will have the ability to possess
        
        DashMovement(1);
        // possession logic
        return;

    case EPlayerPawn::None: // This is only if there is an error, this hopefully should never happen
        
        return;

    default:    // Everyone else will use this ability to leave the possession and return to being a ghost. They will dash in a similar way to the ghost, but backwards
        
        // unpossession logic
        DashMovement(-1);
        return;
    }
}

void AHallowburgePlayerController::Button1Action()
{
    // Punch ability
}

void AHallowburgePlayerController::Button2Action()
{

}

void AHallowburgePlayerController::DashMovement(int PositiveNegativeDirection)
{
    if (bCanDash)
    {
        // Perform dash logic
        FVector ForwardDir = PlayerCharacter->GetActorRotation().Vector();
        PlayerCharacter->LaunchCharacter(ForwardDir * DashDistance * PositiveNegativeDirection, true, true);

        if (DashMontage)
        {
            PlayerCharacter->PlayAnimMontage(DashMontage, 1, NAME_None);
        }

        // Set bCanDash to false to prevent further dashes
        bCanDash = false;

        
        // Start the cooldown timer
        if (PositiveNegativeDirection > 0)
        {
            GetWorld()->GetTimerManager().SetTimer(DashCooldownTimerHandle, this, &AHallowburgePlayerController::DashMovementEnd, DashCooldown, false);
        }
        else
        {
            GetWorld()->GetTimerManager().SetTimer(DashCooldownTimerHandle, this, &AHallowburgePlayerController::DashMovementEnd, DashCooldown * 1.5, false);
        }
    }
}

void AHallowburgePlayerController::DashMovementEnd()
{
    bCanDash = true;
}