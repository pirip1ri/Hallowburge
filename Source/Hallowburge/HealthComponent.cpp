// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/DamageType.h"
#include "PossessableCharacter.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	CurrentHealth = MaxHealth;
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	CurrentHealth = MaxHealth;

}

void UHealthComponent::DeductHealth(APossessableCharacter* DamagedActor, float Damage)
{
	if (Damage <= 0 || CurrentHealth <= 0)
	{
		return;
	}
	CurrentHealth = FMath::Clamp(CurrentHealth - Damage, 0.0f, MaxHealth);

	if (CurrentHealth <= 0.0f)
	{
		UE_LOG(LogTemp, Display, TEXT("Die, die die, die, die!")); 
		DamagedActor->OnDeath();
	}
	UE_LOG(LogTemp, Display, TEXT("CurrentHealth: %f"), CurrentHealth);
}

void UHealthComponent::HandleCollisionDamage(AActor* CollidedActor, float Damage, AController* Controller, AActor* OffenseActor)
{
	if (CollidedActor && CollidedActor != OffenseActor)  // Ensure the other actor is valid and not self
	{
		UE_LOG(LogTemp, Display, TEXT("UHealthComponent::HandleCollisionDamage"));
		// Apply damage
		UGameplayStatics::ApplyDamage(CollidedActor, Damage, Controller, OffenseActor, DamageTypeClass);
	}
}
