// Fill out your copyright notice in the Description page of Project Settings.


#include "GhostPlayerCharacter.h"
#include "HallowburgePlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AIController.h"
#include "HallowburgeSandboxGameModeBase.h"

// Sets default values
AGhostPlayerCharacter::AGhostPlayerCharacter()
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
    CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AGhostPlayerCharacter::PossessCharacterCheck);

	PunchChargeTime = FMath::Clamp(PunchChargeTime, 0.0f, 0.7f);
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
	PunchState = EPunchState::Idle;
}

void AGhostPlayerCharacter::PossessiveDashStart()
{
		bCanPossess = true;
		DashMovement(1);
		GetWorld()->GetTimerManager().SetTimer(PossessionProgressTimerHandle, this, &AGhostPlayerCharacter::PossessiveDashEnd, 0.3f, false); // Check possession progress periodically
}

void AGhostPlayerCharacter::ActionButton1()
{
	if (!bCanPossess)
	{
		if (PunchState == EPunchState::Idle)
		{
			if (GetCharacterMovement()->IsFalling())
			{
				AirPunch();
			}
			else
			{
				Punch();
			}
		}
		PunchState = EPunchState::Charging;
	}
	else
	{
		PunchState = EPunchState::Idle;
		PunchChargeTime = 0.0f;
	}
}

void AGhostPlayerCharacter::ActionButton1End()
{
	UE_LOG(LogTemp, Display, TEXT("%f, %f"), PunchChargeTime, PunchState);
	if (PunchChargeTime >= 0.69f)
	{
		ChargedPunch();
	}
	else if (PunchChargeTime > 0.0f)
	{

	}

	PunchState = EPunchState::Cooldown; // Set to cooldown only if a punch occurred
}

void AGhostPlayerCharacter::PossessCharacterCheck(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
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
		GetWorld()->GetTimerManager().ClearTimer(PossessionProgressTimerHandle);
		PossessiveDashEnd(); // set the bCanPossess boolean immediately to false


		// Try to cast the OtherActor to a possessable character
		APossessableCharacter* PossessableCharacter = Cast<APossessableCharacter>(OtherActor);

		if (PossessableCharacter)  // If the cast was successful
		{

			// Get the player controller - should work for our single player game
			AHallowburgePlayerController* PlayerController = Cast<AHallowburgePlayerController>(GetWorld()->GetFirstPlayerController());


			if (PlayerController)
			{
				// Smoke and Mirrors:
				this->SetActorHiddenInGame(true);  // Make the ghost invisible
				this->GetCharacterMovement()->SetMovementMode(MOVE_None); // disable any character movement
				this->SetActorEnableCollision(false);  // Disable collisions for the ghost
				DisableInput(PlayerController);
				OnPossessCharacterInScene(PossessableCharacter, PlayerController); // AI and camera functionality called at in blueprints

				PossessableCharacter->GhostCharacter = this; // set the ghost character variable to this, so that when the player wants to unpossess, the code can return to this specific ghost in the level
			}
		}
	}
}

void AGhostPlayerCharacter::ChangeControlledCharacter(AHallowburgePlayerController* PlayerController, APossessableCharacter* PossessedCharacter)
{
	// This function gets called in blueprints after OnPossessCharacterInScene is complete
	
	// EnableInput
	EnableInput(PlayerController);
	
	if (PossessedCharacter->GetController()) // Check to see if this possessed character has AI
	{
		AIControllerStash = Cast<AAIController>(PossessedCharacter->GetController()); // if so, stash the AI into a variable to be used later
		AAIController* AIController = Cast<AAIController>(PossessedCharacter->GetController());
		if (AIController)
		{
			AIController->UnPossess();
			UE_LOG(LogTemp, Display, TEXT("AIController successfully unpossessed the pawn."));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("PossessedCharacter or its controller is null at stash time."));
	}

	PlayerController->UnPossess();
	PlayerController->Possess(PossessedCharacter); // Possess the new character

	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	this->AttachToActor(PossessedCharacter, AttachmentRules); // Attach the ghost character to the possessed character

	PlayerController->PlayerCharacter = PossessedCharacter;	// Set the PlayerCharacter to be equal to the possessed pawn's character (this is to keep character input working in the controller)

	PlayerController->SwitchInputMappingContext(PossessedCharacter, 1); 	// Switch to the possessed character's input mapping context
}

void AGhostPlayerCharacter::PossessiveDashEnd()
{
	bCanPossess = false;  // Disable possession after the dash ends
	UE_LOG(LogTemp, Display, TEXT("CanPosess is set to false"));
}

void AGhostPlayerCharacter::Punch()
{
	UE_LOG(LogTemp, Display, TEXT("POW"));
}

void AGhostPlayerCharacter::AirPunch()
{
	UE_LOG(LogTemp, Display, TEXT("BAM"));
}

void AGhostPlayerCharacter::ChargedPunch()
{
	UE_LOG(LogTemp, Display, TEXT("WALLOP"));
}

// Called every frame
void AGhostPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    // Calculate and update CharacterSpeed from movement - for animation
    CharacterSpeed = GetVelocity().Size(); // Total velocity magnitude


	switch (PunchState)
	{
	case EPunchState::Charging:
		PunchChargeTime += DeltaTime;
	break;

	case EPunchState::Cooldown:
		if (PunchChargeTime > 0)
		{
			PunchChargeTime -= DeltaTime;
		}
		else
		{
			PunchState = EPunchState::Idle;
		}
	break;

	case EPunchState::Idle:
	default:
		// Do nothing when idle
	break;
	}
}

// Called to bind functionality to input
void AGhostPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

