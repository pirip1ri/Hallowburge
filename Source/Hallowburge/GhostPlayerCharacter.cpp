// Fill out your copyright notice in the Description page of Project Settings.


#include "GhostPlayerCharacter.h"
#include "HallowburgePlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AIController.h"
#include "HealthComponent.h"
#include "Components/CapsuleComponent.h"
#include "HallowburgeSandboxGameModeBase.h"

// Sets default values
AGhostPlayerCharacter::AGhostPlayerCharacter()
{
	// Create a collision box that will deal with any collision logic
	PossessionCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("PossessionCapsule"));
	PossessionCapsule->SetupAttachment(RootComponent);
	PossessionCapsule->SetCapsuleHalfHeight(70.0f);
	PossessionCapsule->SetCapsuleRadius(65.0f);
	PossessionCapsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);  // Enable query - to detect overlaps 
	PossessionCapsule->SetCollisionObjectType(ECC_Pawn);  // Set this box as a Pawn type for possession checks
	PossessionCapsule->SetCollisionResponseToAllChannels(ECR_Ignore);
	PossessionCapsule->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);  // Overlap with possessable characters
	PossessionCapsule->SetGenerateOverlapEvents(true);
	
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
    PossessionCapsule->OnComponentBeginOverlap.AddDynamic(this, &AGhostPlayerCharacter::OnPossessionOrSpecialPunchOverlap);

	RightHandCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("RightHandCollisionBox"));
	RightHandCollisionBox->SetupAttachment(GetMesh(), TEXT("righthand"));
	RightHandCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RightHandCollisionBox->SetCollisionObjectType(ECollisionChannel::ECC_Pawn); // or whatever is appropriate for your game
	RightHandCollisionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	RightHandCollisionBox->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Overlap); // Set specific channels to overlap
	RightHandCollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AGhostPlayerCharacter::OnPunchOverlap);


	// Initialize the RadialForceComponent
	RadialForceComponent = CreateDefaultSubobject<URadialForceComponent>(TEXT("RadialForceComponent"));
	RadialForceComponent->SetupAttachment(RightHandCollisionBox);
	// Check if RadialForceComponent was successfully created before using it
	if (RadialForceComponent)
	{
		RadialForceComponent->bIgnoreOwningActor = true;
		RadialForceComponent->bImpulseVelChange = true;
	}


	PunchCooldown = FMath::Clamp(PunchCooldown, 0.0f, 0.7f);
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

// Called every frame
void AGhostPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// Calculate and update CharacterSpeed from movement - for animation
	CharacterSpeed = GetVelocity().Size(); // Total velocity magnitude


	switch (PunchState)
	{
	case EPunchState::Cooldown:
		if (PunchCooldown > 0)
		{
			PunchCooldown -= DeltaTime;
		}
		else
		{
			PunchState = EPunchState::Idle;
			PunchCooldown = 0;
		}
		break;
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

void AGhostPlayerCharacter::PossessiveDashStart()
{
	if (PunchState == EPunchState::Idle || PunchState == EPunchState::Cooldown) // if the player is not punching
	{
		PossessionCapsule->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		bCanPossess = true;
		bIsHoldingDownPunchButton = false;
		DashMovement(1);
		GetWorld()->GetTimerManager().SetTimer(PossessionProgressTimerHandle, this, &AGhostPlayerCharacter::PossessiveDashEnd, 0.3f, false); // Check possession progress periodically
	}
}

void AGhostPlayerCharacter::ActionButton1()
{
	UCharacterMovementComponent* CharacterMovementSpeed = GetCharacterMovement();
	CharacterMovementSpeed->MaxWalkSpeed = 300.0f;

	if (!bIsHoldingDownPunchButton)
	{
		bIsHoldingDownPunchButton = true;
		if (!bCanPossess)
		{
			if (PunchState == EPunchState::Idle && PunchCooldown == 0.0f)
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
		}
		else
		{
			PunchState = EPunchState::Idle;
			PunchCooldown = 0.1f;
		}
	}
	
}

void AGhostPlayerCharacter::ActionButton1End()
{
	UE_LOG(LogTemp, Display, TEXT("%f, %f"), PunchCooldown, PunchState);
	if (PunchState == EPunchState::Charging)
	{
		ChargedPunch();
	}
	else
	{
		bIsHoldingDownPunchButton = false;
	}
}

void AGhostPlayerCharacter::OnPossessionOrSpecialPunchOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this)
	{
		if (bIsHoldingDownPunchButton)
		{
			DamageOtherActor(OtherActor);
		}

		else if (bCanPossess)
		{
			// Step 1: Check if the ghost Can Possess

			// Step 2: Possess Target
			// Check if the OtherActor is valid and is not this GhostPlayerCharacter

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

		else
		{
			return;
		}
	}

	else
	{
		return;
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
	PossessionCapsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AGhostPlayerCharacter::Punch()
{
	UE_LOG(LogTemp, Display, TEXT("POW"));
	PunchState = EPunchState::Punch;
	PlayPunchMontage(PunchState);
}

void AGhostPlayerCharacter::AirPunch()
{
	UE_LOG(LogTemp, Display, TEXT("BAM"));
}

void AGhostPlayerCharacter::ChargedPunch()
{
	UE_LOG(LogTemp, Display, TEXT("WALLOP"));
	// Charged Punch logic
	PunchState = EPunchState::SpinAttack;
	PlayPunchMontage(PunchState);
}

void AGhostPlayerCharacter::OnPunchOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Display, TEXT("AGhostPlayerCharacter::OnPunchOverlap 1"));
	if (OtherActor && OtherActor != this)
	{
		UE_LOG(LogTemp, Display, TEXT("AGhostPlayerCharacter::OnPunchOverlap 2"));
		DamageOtherActor(OtherActor);
	}
}

void AGhostPlayerCharacter::EndPunch()
{
	PunchState = EPunchState::Idle;
	PunchCooldown = 0.0f;
	PossessionCapsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AGhostPlayerCharacter::DamageOtherActor(AActor* OtherActor)
{
	if (OtherActor)
	{
		UE_LOG(LogTemp, Log, TEXT("DamageOtherActor called on %s"), *OtherActor->GetName());
		UHealthComponent* HealthComponent = OtherActor->FindComponentByClass<UHealthComponent>();
		if (HealthComponent)
		{
			HealthComponent->HandleCollisionDamage(OtherActor, BaseDamage, GetController(), this);  // Ensure this function handles nulls correctly
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("DamageOtherActor: No HealthComponent found on %s"), *OtherActor->GetName());
		}
	}
	// Set the strength of the radial impulse
	RadialForceComponent->ImpulseStrength = RadialForceStrength;

	// Fire the radial impulse
	RadialForceComponent->FireImpulse();
}

void AGhostPlayerCharacter::PlayPunchMontage_Implementation(EPunchState CurrentPunchState)
{

}



