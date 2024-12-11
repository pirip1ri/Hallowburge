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
    bIsHoldingGun = false;
    bCanShoot = false;

    // Create a skeletal mesh component for the gun
    GunMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMesh"));
    GunMesh->SetupAttachment(GetMesh(), TEXT("RightHandSocket")); // Attach to the right hand socket of the character's skeletal mesh

    // Initialize the projectile spawner component and attach it to the root
    ProjectileSpawner = CreateDefaultSubobject<UProjectileSpawner>(TEXT("ProjectileSpawner"));
    ProjectileSpawner->SetupAttachment(GetMesh(), TEXT("RightHandSocket"));

    // Set the skeletal mesh asset for the gun
    static ConstructorHelpers::FObjectFinder<USkeletalMesh> GunMeshAsset(TEXT("SkeletalMesh'/Game/Path/To/Your/GunMesh.GunMesh'"));
    if (GunMeshAsset.Succeeded())
    {
        GunMesh->SetSkeletalMesh(GunMeshAsset.Object);
    }
}

void AAstronautPlayerCharacter::TriggerDrawGun()
{
    DrawGunAnimation();
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
    GetCharacterMovement()->AirControl = 0.5f;

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
    case EJetpackState::Paused:
        // Transition back to Regenerating when grounded
        if (GetCharacterMovement() && !GetCharacterMovement()->IsFalling())
        {
            JetpackState = EJetpackState::Regenerating;
        }
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
        if (bCanShoot)
        {
            ChargeTime += DeltaTime;
        }
        else
        {
            ChargeTime = 0.0f;
            ChargingState = EChargingShotState::Idle;
        }
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

void AAstronautPlayerCharacter::DrawGunAnimation_Implementation()
{

}

void AAstronautPlayerCharacter::ShootAnimation_Implementation()
{
}

void AAstronautPlayerCharacter::Shoot()
{
    if (bCanShoot && ProjectileSpawner)
    {
        float ProjectileSpeed = FMath::GetMappedRangeValueClamped(FVector2D(MinChargeTime, MaxChargeTime), FVector2D(MinProjectileSpeed, MaxProjectileSpeed), ChargeTime);
        ProjectileSpawner->SpawnProjectile(ShootingProjectile, ProjectileSpeed);

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

        case EJetpackState::Regenerating:
            if (CurrentJetpackFuel >= RefuelJetpackRate)
            {
                JetpackActive();
                ActivateJetpackNiagraEffects();
            }
            else
            {
                Jump();
            }
            break;

        case EJetpackState::Paused:
            // Resume jetpack only if fuel is available
            if (CurrentJetpackFuel > 0)
            {
                JetpackActive();
                ActivateJetpackNiagraEffects();
            }
            break;

        case EJetpackState::Idle:
            // Jetpack is ready to use!
            ActivateJetpackNiagraEffects();
            JetpackActive();
            break;

        default:
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
    DeactivateJetpackNiagraEffects();
}

void AAstronautPlayerCharacter::SetStateToActive()
{
    JetpackActive();
    bIsAITryingToJump = true;
}

void AAstronautPlayerCharacter::ActivateJetpackNiagraEffects_Implementation()
{
}

void AAstronautPlayerCharacter::DeactivateJetpackNiagraEffects_Implementation()
{
}

void AAstronautPlayerCharacter::ActionButton1()
{
    if (!bIsHoldingGun)
    {
        TriggerDrawGun();
        bIsHoldingGun = true;
    }
    else
    {
        ChargingState = EChargingShotState::Charging;
    }
}

void AAstronautPlayerCharacter::ActionButton1End()
{
    UE_LOG(LogTemp, Display, TEXT("AAstronautPlayerCharacter::ActionButton1End called for"));
    ChargingState = EChargingShotState::Cooldown;

    // Clamp the charge time between min and max
    ChargeTime = FMath::Clamp(ChargeTime, MinChargeTime, MaxChargeTime);

    ShootAnimation();
}

void AAstronautPlayerCharacter::JetpackActive()
{
    if (JetpackState == EJetpackState::Empty)
    {
        // Jetpack cannot be used while regenerating or empty
        return;
    }

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

        if (GetCharacterMovement()->IsFalling() && JetpackState == EJetpackState::Active)
        {
            JetpackState = EJetpackState::Paused;
        }
        else
        {
            JetpackState = EJetpackState::Regenerating;
        }
    }
    DeactivateJetpackNiagraEffects();
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
        DeactivateJetpackNiagraEffects();
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
