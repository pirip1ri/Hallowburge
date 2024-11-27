// Fill out your copyright notice in the Description page of Project Settings.


#include "PossessableCharacter.h"
#include "HallowburgePlayerController.h"
#include "GameFramework/Actor.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GhostPlayerCharacter.h"

APossessableCharacter::APossessableCharacter()
{  
    // Disable character rotation by controller pitch
    bUseControllerRotationYaw = true;
    bUseControllerRotationPitch = false;
    bUseControllerRotationRoll = false;

    // Create a Spring Arm component and attach it to the character's root component
    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    SpringArm->SetupAttachment(RootComponent);
    SpringArm->TargetArmLength = 300.0f;
    SpringArm->bUsePawnControlRotation = true; // Rotate the arm based on the controller

    // Create a Camera component and attach it to the Spring Arm
    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
    Camera->bUsePawnControlRotation = true;
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

void APossessableCharacter::DashMovement(float PositiveNegativeDirection)
{
    if (bCanDash)
    {
        UE_LOG(LogTemp, Display, TEXT("APossessableCharacter::DashMovement bCanDash is true"));
        // Perform dash logic
        FVector ForwardDir = GetActorRotation().Vector();
        FVector UpwardDir = FVector::UpVector;  // Character's up vector
        FVector DashVector = (ForwardDir * DashDistance * PositiveNegativeDirection) + (UpwardDir * DashUpwardDistance);
        LaunchCharacter(DashVector, true, true);

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

void APossessableCharacter::PossessiveDashCall()
{
    UE_LOG(LogTemp, Display, TEXT("APossessableCharacter::PossessiveDashCall()"));
    if (GhostCharacter)
	{
		// Get the player controller - should work for our single player game
		AHallowburgePlayerController* PlayerController = Cast<AHallowburgePlayerController>(GetWorld()->GetFirstPlayerController());

        if (PlayerController)
        {
            GhostCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking); // enable movement again for the ghost
            PlayerController->UnPossess();

            if (GhostCharacter->AIControllerStash.IsValid())
            {
                GhostCharacter->AIControllerStash->Possess(this);
                GetCharacterMovement()->bOrientRotationToMovement = false;
                UE_LOG(LogTemp, Display, TEXT("AIController successfully reattached to PossessableCharacter"));
                GhostCharacter->AIControllerStash = nullptr;
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("AIControllerStash is null or invalid in PossessionAbilityCheck!"));
            }

            PlayerController->Possess(GhostCharacter);     // Possess the ghost character again

            PlayerController->PlayerCharacter = GhostCharacter; // Possess the ghost character again (used for the player controller/ input functions)
            PlayerController->SwitchInputMappingContext(GhostCharacter, 0);     // Switch input mapping back to ghost character
            
            GhostCharacter->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform); // Detach ghost player from possessed character
            OnUnpossessCharacterInScene(GhostCharacter);
        }
        GetCharacterMovement()->bOrientRotationToMovement = true;
        this->DashMovement(0.5);
	    GhostCharacter->DashMovement(-0.5);

	    GhostCharacter->SetActorHiddenInGame(false);    // Return the ghost to visibility and enable its collision
	    GhostCharacter->SetActorEnableCollision(true);
        
        GhostCharacter->PlayUnpossessMontage();
        this->PlayStunMontage();

	    GhostCharacter = nullptr;   // Clear the GhostCharacter reference so that this if statement won't happen again unless someone is possessed
	}
}

void APossessableCharacter::OnPossessionOrSpecialPunchOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    UE_LOG(LogTemp, Display, TEXT("APossessableCharacter::ChangePlayerCharacter called for"));
}

void APossessableCharacter::PlayStunMontage_Implementation()
{
}

void APossessableCharacter::OnPossessCharacterInScene_Implementation(APossessableCharacter* PossessedCharacter, AHallowburgePlayerController* PlayerController)
{

}

void APossessableCharacter::OnUnpossessCharacterInScene_Implementation(APossessableCharacter* PossessedCharacter)
{

}

void APossessableCharacter::OnDeath_Implementation()
{

}

