// Fill out your copyright notice in the Description page of Project Settings.


#include "HallowburgePlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "InputMappingContext.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include <Kismet/GameplayStatics.h>

void AHallowburgePlayerController::BeginPlay()
{
    Super::BeginPlay();

    if (CameraComponent)
    {
        FViewTargetTransitionParams TransitionParams;
        SetViewTarget(this, TransitionParams);
        UE_LOG(LogTemp, Warning, TEXT("CameraComponent is active!"));
    }
    
    // Ensure the character is valid and get the character movement component
    if (APawn* ControlledPawn = GetPawn())
    {
        // Cast to APossessableCharacter instead of a specific subclass
        PlayerCharacter = Cast<APossessableCharacter>(ControlledPawn);

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
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Controlled Pawn is not of type APossessableCharacter!"));
        }
    }
    SetPlayersCurrentPlayState(EPlayersCurrentPlayState::Playing);
}

void AHallowburgePlayerController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

AHallowburgePlayerController::AHallowburgePlayerController()
{
    // Create Root Component
    USceneComponent* RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
    RootComponent = RootSceneComponent;

    // Create Spring Arm
    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    SpringArm->SetupAttachment(RootComponent);
    SpringArm->TargetArmLength = 500.0f; // Set a larger default arm length
    SpringArm->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f)); // Raise the spring arm above the root
    SpringArm->bUsePawnControlRotation = true;

    // Create Camera
    CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    CameraComponent->SetupAttachment(SpringArm);
    CameraComponent->bUsePawnControlRotation = false;

    SpringArm->bHiddenInGame = false;
    CameraComponent->bHiddenInGame = false;
}

void AHallowburgePlayerController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);
    if (SpringArm)
    {
        // Attach the spring arm to the possessed pawn
        SpringArm->AttachToComponent(InPawn->GetRootComponent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
        SpringArm->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f)); // Adjust based on your game
        UE_LOG(LogTemp, Display, TEXT("yoyoyoy"));
    }
}

void AHallowburgePlayerController::OnUnPossess()
{
    Super::OnUnPossess();

    // Reset spring arm to default position
    SpringArm->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
    UE_LOG(LogTemp, Display, TEXT("oyoyoy"));

}

void AHallowburgePlayerController::SwitchInputMappingContext_Implementation(APossessableCharacter* NewPossessedCharacter, int Priority)
{
    // Leave empty, do this in Blueprints
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
    if (PlayerCharacter && PlayerCharacter->GetCharacterMovement())
    {
        PlayerCharacter->GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("PlayerCharacter or its movement component is not valid!"));
    }
}

void AHallowburgePlayerController::SprintEnd()
{
    if (PlayerCharacter && PlayerCharacter->GetCharacterMovement())
    {
        PlayerCharacter->GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("PlayerCharacter or its movement component is not valid!"));
    }
}

void AHallowburgePlayerController::PossessionAbilityCheck()
{
    if (PlayerCharacter)
    {
        
        PlayerCharacter->PossessiveDashCall();
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

EPlayersCurrentPlayState AHallowburgePlayerController::GetPlayersCurrentPlayState()
{
    return PlayersCurrentPlayState;
}

void AHallowburgePlayerController::SetPlayersCurrentPlayState(EPlayersCurrentPlayState PlayersPlayState)
{
    PlayersCurrentPlayState = PlayersPlayState;
}
