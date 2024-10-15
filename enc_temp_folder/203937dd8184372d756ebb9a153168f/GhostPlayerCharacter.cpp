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
	bCanDash = true;
}

void AGhostPlayerCharacter::PossessionAbilityCheck()
{
		UE_LOG(LogTemp, Display, TEXT("AGhostPlayerCharacter::PossessionAbilityCheck()"));
		bCanPossess = true;
		DashMovement(1);
		GetWorld()->GetTimerManager().SetTimer(PossessionProgressTimerHandle, this, &AGhostPlayerCharacter::EndPossessionCheck, 0.3f, false); // Check possession progress periodically
}

void AGhostPlayerCharacter::PossessEnemyCharacter(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Step 1: Check if the ghost Can Possess
	//if (!bCanPossess)
	//{
	//	UE_LOG(LogTemp, Display, TEXT("Cannot possess enemy")); 
	//	return;
	//}

	// Step 2: Possess Target
	// Check if the OtherActor is valid and is not this GhostPlayerCharacter
	if (OtherActor && OtherActor != this)
	{
		GetWorld()->GetTimerManager().ClearTimer(PossessionProgressTimerHandle);
		EndPossessionCheck(); // set the bCanPossess boolean immediately to false


		// Try to cast the OtherActor to a possessable character
		APossessableCharacter* PossessableCharacter = Cast<APossessableCharacter>(OtherActor);

		if (PossessableCharacter)  // If the cast was successful
		{

			// Get the player controller - should work for our single player game
			AHallowburgePlayerController* PlayerController = Cast<AHallowburgePlayerController>(GetWorld()->GetFirstPlayerController());


			if (PlayerController)
			{
				PlayerController->UnPossess();
				PlayerController->Possess(PossessableCharacter); // Possess the new character

				FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true); 
				this->AttachToActor(PossessableCharacter, AttachmentRules); // attach the ghost character to the possessed character
				
				this->SetActorHiddenInGame(true);  // Make the ghost invisible
				this->SetActorEnableCollision(false);  // Disable collisions for the ghost

				PlayerController->PlayerCharacter = PossessableCharacter;	// Set the PlayerCharacter to be equal to the possessed pawn's character (this is to keep character input working in the controller)
				
				PlayerController->SwitchInputMappingContext(PossessableCharacter, 1); 	// Switch to the possessed character's input mapping context

				PossessableCharacter->GhostCharacter = this; // set the ghost character variable to this, so that when the player wants to unpossess, the code can return to this specific ghost in the level
			}
		}
	}
}

void AGhostPlayerCharacter::EndPossessionCheck()
{
	bCanPossess = false;  // Disable possession after the dash ends
	UE_LOG(LogTemp, Display, TEXT("CanPosess is set to false"));
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

