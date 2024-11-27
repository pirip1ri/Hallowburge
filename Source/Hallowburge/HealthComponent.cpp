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
	// Initialize default values
	MaxHealth = 3.0f;
	CurrentHealth = MaxHealth;
	AttackTokenCount = 2; // Example default value
}


bool UHealthComponent::ReserveAttackToken(int Amount)
{
	if (AttackTokenCount >= Amount)
	{
		AttackTokenCount -= Amount;
		return true;
	}
	else
	{
		return false;
	}
}

void UHealthComponent::ReturnAttackToken(int Amount)
{
	AttackTokenCount += Amount;
}

// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	CurrentHealth = MaxHealth;
	if (CurrentHealth <= 0) // Ensure CurrentHealth is valid
	{
		UE_LOG(LogTemp, Display, TEXT("UHealthComponent - Maximum Health has not set up properly for a character."));
		CurrentHealth = 3;
	}

	if (AttackTokenCount <= 0) // Set a minimum valid value
	{
		UE_LOG(LogTemp, Display, TEXT("UHealthComponent - AttackTokenCount has not been set up properly for a character."));
		AttackTokenCount = 2; // Example default
	}

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
