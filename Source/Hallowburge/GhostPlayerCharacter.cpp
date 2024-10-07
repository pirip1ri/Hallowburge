// Fill out your copyright notice in the Description page of Project Settings.


#include "GhostPlayerCharacter.h"
#include "HallowburgePlayerController.h"
#include "HallowburgeSandboxGameModeBase.h"

// Sets default values
AGhostPlayerCharacter::AGhostPlayerCharacter()
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
    CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AGhostPlayerCharacter::PossessEnemyCharacter);
}

// Called when the game starts or when spawned
void AGhostPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
    if (GetCharacterMovement())
    {
        MaxWalkSpeed = GetCharacterMovement()->MaxWalkSpeed;
    }

    // Allows the character to move easily in the air while jumping
    GetCharacterMovement()->AirControl = 0.8f;

    GameModeRef = Cast<AHallowburgeSandboxGameModeBase>(GetWorld()->GetAuthGameMode());
	bCanPossess = false;
}

void AGhostPlayerCharacter::PossessionAbilityCheck()
{
    bCanPossess = true;
}

void AGhostPlayerCharacter::PossessEnemyCharacter(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Step 1: Check if the ghost Can Possess
	if (!bCanPossess)
	{
		UE_LOG(LogTemp, Display, TEXT("Cannot possess enemy")); 
		return;
	}

	// Step 2: Possess Target
	// Check if the OtherActor is valid and is not this GhostPlayerCharacter
	if (OtherActor && OtherActor != this)
	{
		// Try to cast the OtherActor to a possessable character
		APossessableCharacter* PossessableCharacter = Cast<APossessableCharacter>(OtherActor);

		if (PossessableCharacter)  // If the cast was successful
		{

			// Get the player controller (assuming it's a single-player game, this gets the first controller)
			APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();

			if (PlayerController)
			{
				// Possess the new character
				PlayerController->Possess(PossessableCharacter);

				// Optionally hide the ghost character
				this->SetActorHiddenInGame(true);  // Make the ghost invisible
				this->SetActorEnableCollision(false);  // Disable collisions for the ghost
			}
		}
	}
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

