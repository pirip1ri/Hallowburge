// Fill out your copyright notice in the Description page of Project Settings.


#include "PossessableCharacter.h"

// Sets default values
APossessableCharacter::APossessableCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APossessableCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APossessableCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APossessableCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void APossessableCharacter::DashMovement(int PositiveNegativeDirection)
{
    if (bCanDash)
    {
        // Perform dash logic
        FVector ForwardDir = GetActorRotation().Vector();
        LaunchCharacter(ForwardDir * DashDistance * PositiveNegativeDirection, true, true);

        if (DashMontage)
        {
            PlayAnimMontage(DashMontage, 1, NAME_None);
        }

        // Set bCanDash to false to prevent further dashes
        bCanDash = false;


        // Start the cooldown timer
        if (PositiveNegativeDirection > 0)
        {
            GetWorld()->GetTimerManager().SetTimer(DashCooldownTimerHandle, this, &APossessableCharacter::DashMovementEnd, DashCooldown, false);
        }
        else
        {
            GetWorld()->GetTimerManager().SetTimer(DashCooldownTimerHandle, this, &APossessableCharacter::DashMovementEnd, DashCooldown * 1.5, false);
        }
    }
}

void APossessableCharacter::DashMovementEnd()
{
    bCanDash = true;
}

void APossessableCharacter::JumpFunction()
{
    Jump();
}

void APossessableCharacter::JumpFunctionEnd()
{
    StopJumping();
}

void APossessableCharacter::PossessionAbilityCheck()
{

}

void APossessableCharacter::ChangePlayerCharacter()
{
    /*  // Potential Logic for possessing
    if (TargetEnemy)
    {
        // Transfer control to the target enemy
        GetController()->Possess(TargetEnemy);

        // Update enum state to reflect the new possession
        PlayerPawn = EPlayerPawn::Astronaut;  // Example: assuming "Astronaut" is the possessed form
    } 
    
    
        // Potential Logic for unpossessing

        // Return control to the main player character
    APlayerController* PlayerController = Cast<APlayerController>(GetController());
    if (PlayerController)
    {
        PlayerController->Possess(this);  // Possess the original player character
    }

    // Update enum state to reflect the player's original form
    PlayerPawn = EPlayerPawn::Ghost; */
}

