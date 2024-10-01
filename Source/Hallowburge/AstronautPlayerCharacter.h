// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PossessableCharacter.h"
#include "AstronautPlayerCharacter.generated.h"

UENUM(BlueprintType)
enum class EJetpackState : uint8
{
	Idle        UMETA(DisplayName = "Idle"),         // No activity
	Active      UMETA(DisplayName = "Active"),		 // Jetpack is being used
	Empty		UMETA(DisplayName = "Empty"),		 // No more fuel
	Regenerating UMETA(DisplayName = "Regenerating") // Jetpack fuel is regenerating
};

UCLASS()
class HALLOWBURGE_API AAstronautPlayerCharacter : public APossessableCharacter
{
	GENERATED_BODY()

public:

	// Fuel amount for the jetpack (public or protected depending on design)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jetpack")
	float MaxJetpackFuel = 50.0f;
	float CurrentJetpackFuel = MaxJetpackFuel;
	float RefuelJetpackRate = 10.0f;
	UPROPERTY(BlueprintReadOnly)
	EJetpackState JetpackState = EJetpackState::Idle;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void JetpackActive();
	void JetpackDeactivate();
	// Private helper to handle jetpack fuel consumption
	void ConsumeJetpackFuel(float DeltaTime);
	void RefuelJetpack(float DeltaTime);


	// Sets default values for this character's properties
	AAstronautPlayerCharacter();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void JumpFunction() override;
	virtual void JumpFunctionEnd() override;
};
