// Fill out your copyright notice in the Description page of Project Settings.


#include "AstronautPlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "HallowburgeSandboxGameModeBase.h"
#include <Kismet/GameplayStatics.h>

// Sets default values
AAstronautPlayerCharacter::AAstronautPlayerCharacter()
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    // Initialize the projectile spawner component and attach it to the root
    ProjectileSpawner = CreateDefaultSubobject<UProjectileSpawner>(TEXT("ProjectileSpawner"));
    ProjectileSpawner->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AAstronautPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
    if (GetCharacterMovement())
    {
        MaxWalkSpeed = GetCharacterMovement()->MaxWalkSpeed;
    }

    // Allows the character to move easily in the air while jumping
    GetCharacterMovement()->AirControl = 0.8f;

    GameModeRef = Cast<AHallowburgeSandboxGameModeBase>(GetWorld()->GetAuthGameMode());
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

    // Calculate and update CharacterSpeed from movement - for animation
    CharacterSpeed = GetVelocity().Size(); // Total velocity magnitude

    switch (ChargingState)
    {
    case EChargingShotState::Charging:
        ChargeTime += DeltaTime;
        break;

    case EChargingShotState::Cooldown:
        if (ChargeTime > 0)
        {
            ChargeTime -= DeltaTime;
            break;
        }
        else
        {
            ChargingState = EChargingShotState::Idle;
            break;
        }
    case EChargingShotState::Idle:
    default:
        // Do nothing when idle
        break;
    }

    if (bIsAITryingToJump)
    {
        ControlAIJetpackMovement(DestinationDistanceFromSelf);
    }
}

void AAstronautPlayerCharacter::Shoot()
{
    if (bCanShoot && ProjectileSpawner)
    {
        float ProjectileSpeed = FMath::GetMappedRangeValueClamped(FVector2D(MinChargeTime, MaxChargeTime), FVector2D(MinProjectileSpeed, MaxProjectileSpeed), ChargeTime);
        ProjectileSpawner->SpawnProjectile(ShootingProjectile, ProjectileSpeed);
        //UE_LOG(LogTemp, Display, TEXT("Projectile Speed: %f"), ProjectileSpeed);

        // Start cooldown before next shot can be fired
        bCanShoot = false;
        GetWorld()->GetTimerManager().SetTimer(ShootingCooldownTimerHandle, this, &AAstronautPlayerCharacter::ResetShootingCooldown, ProjectileSpawnerCooldown, false);
    }
}

void AAstronautPlayerCharacter::ResetShootingCooldown()
{
    bCanShoot = true;
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

void AAstronautPlayerCharacter::JumpFunctionEnd()
{
    if (JetpackState == EJetpackState::Active)
    {
        JetpackDeactivate();
    }
    else
    {
        // Stop the regular jump if jetpack isn't active
        StopJumping();
    }
}

void AAstronautPlayerCharacter::SetStateToActive()
{
    JetpackActive();
    bIsAITryingToJump = true;
}

void AAstronautPlayerCharacter::ActionButton1()
{
    ChargingState = EChargingShotState::Charging;
}

void AAstronautPlayerCharacter::ActionButton1End()
{
    ChargingState = EChargingShotState::Cooldown;

    // Clamp the charge time between min and max
    ChargeTime = FMath::Clamp(ChargeTime, MinChargeTime, MaxChargeTime);

    Shoot();
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

void AAstronautPlayerCharacter::ControlAIJetpackMovement(float VectorDistance)
{
    if (VectorDistance >= 280.0f || VectorDistance <= 30.0f)
    {
        JumpFunctionEnd();
    }
}
