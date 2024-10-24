// Fill out your copyright notice in the Description page of Project Settings.


#include "ShootingProjectile.h"
#include <Kismet/GameplayStatics.h>

// Sets default values
AShootingProjectile::AShootingProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	RootComponent = CollisionBox; // Assigned as root component
	//CollisionBox->OnComponentHit.AddDynamic(this, &AShootingProjectile::OnHit);

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	ProjectileMesh->SetupAttachment(CollisionBox); // Attach a static mesh so it is visible

	ParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystem"));
	ParticleSystem->SetupAttachment(CollisionBox); // Attach a particle system for visual flair

	RadialForce = CreateDefaultSubobject<URadialForceComponent>(TEXT("RadialForce"));
	RadialForce->SetupAttachment(CollisionBox); // Attach this so that, when it collides with something, a bit of force is felt


	ProjectileMovementComponent = CreateDefaultSubobject <UProjectileMovementComponent>(TEXT("Projectile Component"));

}

// Called when the game starts or when spawned
void AShootingProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AShootingProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
