// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageComponent.h"
#include "GameFramework/Actor.h"
#include "HealthComponent.h"
#include "DamageComponent.h"
#include "PossessableCharacter.h"

// Sets default values for this component's properties
UDamageComponent::UDamageComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UDamageComponent::ApplyDamage(APossessableCharacter* Target)
{
	if (!Target) return;

	UE_LOG(LogTemp, Display, TEXT(""));
	UHealthComponent* HealthComp = Target->FindComponentByClass<UHealthComponent>();
	if (HealthComp)
	{

	}
}


// Called when the game starts
void UDamageComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

