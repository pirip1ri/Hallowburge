// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "PossessableCharacter.generated.h"

UENUM(BlueprintType)
enum class EPlayerPawn : uint8
{
	None		UMETA(DisplayName = "None"),
	Ghost		UMETA(DisplayName = "Ghost"),
	Astronaut	UMETA(DisplayName = "Astronaut")
};

class AHallowburgeSandboxGameModeBase;
class AHallowburgePlayerController;

UCLASS()
class HALLOWBURGE_API APossessableCharacter : public ACharacter
{
	GENERATED_BODY()

	// Parameters //
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* Camera;

	AHallowburgeSandboxGameModeBase* GameModeRef;

	EPlayerPawn PlayerPawn;

	// Objects - Animation stuff //

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float CharacterSpeed;
	float MaxWalkSpeed = 600.0f;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash")
	bool bCanDash;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash")
	float DashCooldown;
	float DashDistance = 1800.0f;
	FTimerHandle DashCooldownTimerHandle;

	// For Animations
	UPROPERTY(EditAnywhere)
	UAnimMontage* DashMontage;


	// Functions //

	// Sets default values for this character's properties
	APossessableCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void JumpFunction();
	virtual void JumpFunctionEnd();
	virtual void PossessionAbilityCheck(); // maybe add variable APossessableCharacter* OtherActor
	virtual void ActionButton1() {}
	virtual void ActionButton2() {}
	virtual void ActionButton1End() {}
	virtual void ActionButton2End() {}

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void DashMovement(int PositiveNegativeDirection);
	void DashMovementEnd();
	void ChangePlayerCharacter(); // maybe add variable APossessableCharacter* TargetCharachter
};
