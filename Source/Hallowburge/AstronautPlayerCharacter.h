// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ProjectileSpawner.h"
#include "ShootingProjectile.h"
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

UENUM()
enum class EChargingShotState : uint8
{
	Idle			UMETA(DisplayName = "Idle"),
	Charging        UMETA(DisplayName = "Charging"),
	Cooldown		UMETA(DisplayName = "Cooldown")
};

UCLASS()
class HALLOWBURGE_API AAstronautPlayerCharacter : public APossessableCharacter
{
	GENERATED_BODY()

	// Parameters //

public:

	// Fuel amount for the jetpack (public or protected depending on design)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Astronaut")
	float MaxJetpackFuel = 50.0f;
	float CurrentJetpackFuel = MaxJetpackFuel;
	UPROPERTY(EditAnywhere, Category = "Astronaut")
	float RefuelJetpackRate = 10.0f;
	UPROPERTY(BlueprintReadOnly)
	EJetpackState JetpackState = EJetpackState::Idle;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bIsAITryingToJump;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float DestinationDistanceFromSelf;


	UPROPERTY(EditAnywhere, Category = "Astronaut")
	TSubclassOf<AShootingProjectile> ShootingProjectile;
	UPROPERTY(EditAnywhere, Category = "Astronaut")
	float ChargeTime = 0.0f;
	const float MinChargeTime = 0.0f; // Minimum charge time (lower bound)
	UPROPERTY(EditAnywhere, Category = "Astronaut")
	float MaxChargeTime = 2.0f; // Maximum charge time (upper bound)
	UPROPERTY(EditAnywhere, Category = "Astronaut")
	float MinProjectileSpeed = 2000.0f; // Speed at minimum charge
	UPROPERTY(EditAnywhere, Category = "Astronaut")
	float MaxProjectileSpeed = 5000.0f; // Speed at maximum charge
	UPROPERTY(BlueprintReadWrite)
	bool bIsHoldingGun;



	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UProjectileSpawner* ProjectileSpawner;
	UPROPERTY(EditAnywhere, Category = "Astronaut")
	float ProjectileSpawnerCooldown = 0.3f;

	UPROPERTY(BlueprintReadWrite, Category = "Astronaut")
	bool bCanShoot;
	EChargingShotState ChargingState = EChargingShotState::Idle;
	UPROPERTY(BlueprintReadWrite)
	FTimerHandle ShootingCooldownTimerHandle;


	// Functions //


protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void JetpackActive();
	void JetpackDeactivate();
	// Private helper to handle jetpack fuel consumption
	void ConsumeJetpackFuel(float DeltaTime);
	void RefuelJetpack(float DeltaTime);
	void ControlAIJetpackMovement(float VectorDistance);


	// Sets default values for this character's properties
	AAstronautPlayerCharacter();

	UFUNCTION(BlueprintCallable)
	void TriggerDrawGun();
	UFUNCTION(BlueprintNativeEvent)
	void DrawGunAnimation();
	UFUNCTION(BlueprintNativeEvent)
	void ShootAnimation();
	UFUNCTION(BlueprintCallable)
	void Shoot();
	void ResetShootingCooldown();
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void JumpFunction() override;
	virtual void JumpFunctionEnd() override;

	UFUNCTION(BlueprintCallable)
	void SetStateToActive();

	virtual void ActionButton1() override;
	virtual void ActionButton1End() override;

};
