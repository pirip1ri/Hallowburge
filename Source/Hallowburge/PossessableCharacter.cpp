// Fill out your copyright notice in the Description page of Project Settings.


#include "PossessableCharacter.h"
#include "Components/CapsuleComponent.h"
#include "HallowburgePlayerController.h"
#include "GhostPlayerCharacter.h"


APossessableCharacter::APossessableCharacter()
{
    // Create a collision box that will deal with any collision logic
    CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
    CollisionBox->SetupAttachment(RootComponent); // Attach to the root component
    CollisionBox->SetBoxExtent(FVector(50.0f, 50.0f, 150.0f)); // Default size
    CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);  // Enable query - to detect overlaps 
    CollisionBox->SetCollisionObjectType(ECC_Pawn);  // Set this box as a Pawn type for possession checks
    CollisionBox->SetCollisionResponseToAllChannels(ECR_Ignore);
    CollisionBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);  // Overlap with possessable characters
    CollisionBox->SetGenerateOverlapEvents(true);

    
    // Create a Spring Arm component and attach it to the character's root component
    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    SpringArm->SetupAttachment(CollisionBox);
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
        UE_LOG(LogTemp, Display, TEXT("APossessableCharacter::DashMovement bCanDash is true"));
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
    UE_LOG(LogTemp, Display, TEXT("APossessableCharacter::PossessionAbilityCheck() called for"));
    if (GhostCharacter)
    {
        // Get the player controller - should work for our single player game
        AHallowburgePlayerController* PlayerController = Cast<AHallowburgePlayerController>(GetWorld()->GetFirstPlayerController());
        UE_LOG(LogTemp, Display, TEXT("APossessableCharacter::PossessionAbilityCheck() GhostCharacter exists"));

        if (PlayerController)
        {
            UE_LOG(LogTemp, Display, TEXT("APossessableCharacter::PossessionAbilityCheck() has player controller"));
            GhostCharacter->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform); // Detach ghost player from possessed character

            this->DashMovement(1);
            GhostCharacter->DashMovement(-1);

            GhostCharacter->SetActorHiddenInGame(false);    // Return the ghost to visibility and enable its collision
            GhostCharacter->SetActorEnableCollision(true);


            PlayerController->Possess(GhostCharacter);     // Possess the ghost character again

            PlayerController->PlayerCharacter = GhostCharacter; // Possess the ghost character again
            PlayerController->SwitchInputMappingContext(GhostCharacter, 0);     // Switch input mapping back to ghost character

            GhostCharacter = nullptr;   // Clear the GhostCharacter reference so that this if statement won't happen again unless someone is possessed

        }
    }
}

void APossessableCharacter::PossessEnemyCharacter(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    UE_LOG(LogTemp, Display, TEXT("APossessableCharacter::ChangePlayerCharacter called for"));
}

