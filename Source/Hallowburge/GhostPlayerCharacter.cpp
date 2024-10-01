// Fill out your copyright notice in the Description page of Project Settings.


#include "GhostPlayerCharacter.h"
#include "HallowburgePlayerController.h"
#include "HallowburgeSandboxGameModeBase.h"

// Sets default values
AGhostPlayerCharacter::AGhostPlayerCharacter()
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
void AGhostPlayerCharacter::BeginPlay()
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

void AGhostPlayerCharacter::PossessionAbilityCheck()
{

}

// Called every frame
void AGhostPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    // Calculate and update CharacterSpeed from movement - for animation
    CharacterSpeed = GetVelocity().Size(); // Total velocity magnitude

}

// Called to bind functionality to input
void AGhostPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

