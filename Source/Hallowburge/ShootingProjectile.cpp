// Fill out your copyright notice in the Description page of Project Settings.


#include "ShootingProjectile.h"
#include <Kismet/GameplayStatics.h>

// Sets default values
AShootingProjectile::AShootingProjectile()
{
	// Disable Tick as it's not required for now
	PrimaryActorTick.bCanEverTick = false;

	// Initialize the LaserProjectile as the root component
	LaserProjectile = CreateDefaultSubobject<UNiagaraComponent>(TEXT("LaserProjectile"));
	RootComponent = LaserProjectile;

	// Initialize and attach the collision box
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->SetupAttachment(LaserProjectile);
	CollisionBox->SetCollisionProfileName("BlockAll");

	// Initialize and attach LaserProjectileEffects
	LaserProjectileEffects = CreateDefaultSubobject<UNiagaraComponent>(TEXT("LaserProjectileEffects"));
	LaserProjectileEffects->SetupAttachment(LaserProjectile);

	// Initialize and attach LaserHitEffect
	LaserHitEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("LaserHitEffect"));
	LaserHitEffect->SetupAttachment(LaserProjectile);

	RadialForce = CreateDefaultSubobject<URadialForceComponent>(TEXT("RadialForce"));
	RadialForce->SetupAttachment(CollisionBox); // Attach this so that, when it collides with something, a bit of force is felt

	ProjectileMovementComponent = CreateDefaultSubobject <UProjectileMovementComponent>(TEXT("Projectile Component"));
}

// Called when the game starts or when spawned
void AShootingProjectile::BeginPlay()
{
	Super::BeginPlay();

	// Optionally bind collision events if needed
	// CollisionBox->OnComponentHit.AddDynamic(this, &AShootingProjectile::OnHit);
}

// Called every frame
void AShootingProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Handle collision events
void AShootingProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Handle collision logic here, such as triggering LaserHitEffect
}
