// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileSpawner.h"
#include "ShootingProjectile.h"

// Sets default values
UProjectileSpawner::UProjectileSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryComponentTick.bCanEverTick = false;
}

void UProjectileSpawner::SpawnProjectile(TSubclassOf<AShootingProjectile> ShootingProjectile, float ChargeBoost)
{
    if (!ShootingProjectile || !GetOwner()) return; // if no actor or shooting projectile, ignore this call

    FVector SpawnLocation = GetComponentLocation(); // get spawner location
    FRotator SpawnRotation = GetComponentRotation(); // get spawner rotation (for aim)

    AShootingProjectile* SpawnedProjectile = GetWorld()->SpawnActor<AShootingProjectile>(ShootingProjectile, SpawnLocation, SpawnRotation); // spawn the projectile
    // if spawn succeed
    if (SpawnedProjectile != nullptr)
    {
        SpawnedProjectile->SetLifeSpan(3.0f);

        if (SpawnedProjectile)
        {
            APawn* OwnerPawn = Cast<APawn>(GetOwner()); // get pawn
            if (OwnerPawn && OwnerPawn->GetController()) // if there is a pawn and a controller
            {
                FRotator ControlRotation = OwnerPawn->GetController()->GetControlRotation();
                FVector ShotDirection = ControlRotation.Vector(); // get the direction of where the camera is looking

                // Apply velocity to the projectile in the direction the player is aiming
                SpawnedProjectile->ProjectileMovementComponent->Velocity = ShotDirection * ChargeBoost;
            }
        }
    }
}

