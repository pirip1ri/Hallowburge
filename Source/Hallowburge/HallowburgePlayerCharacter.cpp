// Fill out your copyright notice in the Description page of Project Settings.


#include "HallowburgePlayerCharacter.h"
#include "HallowburgePlayerController.h"
#include "HallowburgeSandboxGameModeBase.h"

void AHallowburgePlayerCharacter::JumpFunction()
{
}

// Sets default values
AHallowburgePlayerCharacter::AHallowburgePlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    // Create a Spring Arm component and attach it to the character's root component
    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    SpringArm->SetupAttachment(RootComponent);
    SpringArm->TargetArmLength = 300.0f;
    SpringArm->bUsePawnControlRotation = true; // Rotate the arm based on the controller

    // Create a Camera component and attach it to the Spring Arm
    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
    Camera->bUsePawnControlRotation = true;

    // Disable character rotation by controller pitch
    bUseControllerRotationYaw = true;
    bUseControllerRotationPitch = false;
    bUseControllerRotationRoll = false;
}

// Called when the game starts or when spawned
void AHallowburgePlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
    PlayerPawn = EPlayerPawn::Ghost;
    if (GetCharacterMovement())
    {
        MaxWalkSpeed = GetCharacterMovement()->MaxWalkSpeed;
    }

    // Allows the character to move easily in the air while jumping
    GetCharacterMovement()->AirControl = 0.8f;

    GameModeRef = Cast<AHallowburgeSandboxGameModeBase>(GetWorld()->GetAuthGameMode());
}

// Called every frame
void AHallowburgePlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    // Calculate and update CharacterSpeed from movement - for animation
    CharacterSpeed = GetVelocity().Size(); // Total velocity magnitude

}
//
//void AHallowburgePlayerCharacter::Landed(const FHitResult& Hit)
//{
//    Super::Landed(Hit);
//
//    // Get reference to the player controller
//    AHallowburgePlayerController* PlayerController = Cast<AHallowburgePlayerController>(GetController());
//
//    if (PlayerController)
//    {
//        // You can now safely modify JetpackState from the player controller
//        if (PlayerController->JetpackState != EJetpackState::Idle)
//        {
//
//            PlayerController->JetpackState = EJetpackState::Regenerating;
//        }
//    }
//}

// Called to bind functionality to input
void AHallowburgePlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AHallowburgePlayerCharacter::PossessEnemy(ACharacter* TargetEnemy)
{
    if (TargetEnemy)
    {
        // Transfer control to the target enemy
        GetController()->Possess(TargetEnemy);

        // Update enum state to reflect the new possession
        PlayerPawn = EPlayerPawn::Astronaut;  // Example: assuming "Astronaut" is the possessed form
    }
}

void AHallowburgePlayerCharacter::UnpossessEnemy()
{
    // Return control to the main player character
    APlayerController* PlayerController = Cast<APlayerController>(GetController());
    if (PlayerController)
    {
        PlayerController->Possess(this);  // Possess the original player character
    }

    // Update enum state to reflect the player's original form
    PlayerPawn = EPlayerPawn::Ghost;
}

