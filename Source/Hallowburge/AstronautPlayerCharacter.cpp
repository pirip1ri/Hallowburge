// Fill out your copyright notice in the Description page of Project Settings.


#include "AstronautPlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include <Kismet/GameplayStatics.h>

// Sets default values
AAstronautPlayerCharacter::AAstronautPlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAstronautPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAstronautPlayerCharacter::Tick(float DeltaTime)
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

// Called to bind functionality to input
void AAstronautPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AAstronautPlayerCharacter::JumpFunction()
{
    if (GetCharacterMovement()->IsFalling())
    {
        switch (JetpackState)
        {
        case EJetpackState::Empty:
            // Jetpack is empty, fallback to normal jump behavior
            Jump();
            break;

        case EJetpackState::Regenerating:
            // Jetpack is inactive when refueling, so cannot do anything but stick with the regular jump logic
            Jump();
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
        Jump();
    }
}

void AAstronautPlayerCharacter::JetpackActive()
{
    if (GetCharacterMovement() && CurrentJetpackFuel > 0)
    {
        JetpackState = EJetpackState::Active; // Set to active to consume fuel
        GetCharacterMovement()->GravityScale = 0.0f; // Disable gravity
        FVector Velocity = GetVelocity();
        FVector NewVelocity = FVector(Velocity.X, Velocity.Y, 250.0f);
        GetCharacterMovement()->Velocity = NewVelocity;
    }
}

void AAstronautPlayerCharacter::JetpackDeactivate()
{
    if (GetCharacterMovement())
    {
        // Return gravity to normal
        GetCharacterMovement()->GravityScale = 1.0f;

        // Switch back to Idle state when jetpack is deactivated
        JetpackState = EJetpackState::Regenerating; // was originally idle
    }
}

void AAstronautPlayerCharacter::ConsumeJetpackFuel(float DeltaTime)
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

void AAstronautPlayerCharacter::RefuelJetpack(float DeltaTime)
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
