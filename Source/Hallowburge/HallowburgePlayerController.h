#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "InputActionValue.h"
#include "EnhancedInputSubsystems.h"
#include "GhostPlayerCharacter.h"
#include "HallowburgeSandboxGameModeBase.h"
#include "GameFramework/PlayerController.h"
#include "HallowburgePlayerController.generated.h"

UCLASS()
class HALLOWBURGE_API AHallowburgePlayerController : public APlayerController
{
	GENERATED_BODY()

	
	// Universal Parameters //
public:
	AHallowburgeSandboxGameModeBase* GameModeRef;

protected:
	APossessableCharacter* PlayerCharacter;

	float RunSpeed = 600.0f;
	float SprintSpeed = 900.0f;
	float MaxWalkSpeed;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash")
	bool bCanDash;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash")
	float DashCooldown;
	float DashDistance = 1800.0f;
	FTimerHandle DashCooldownTimerHandle;

	/** Base lookup rate, in deg/sec. Other scaling may affect final lookup rate. */
	UPROPERTY(EditAnywhere, Category = "Look")
	float BaseLookUpRate = 90.0f;

	/** Base lookright rate, in deg/sec. Other scaling may affect final lookup rate. */
	UPROPERTY(EditAnywhere, Category = "Look")
	float BaseLookRightRate = 90.0f;

	// For Animations
	UPROPERTY(EditAnywhere)
	UAnimMontage* DashMontage;



	// Universal Functions //
public:
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;



protected:
	void SetupInputComponent() override;

	void LookUp(const FInputActionValue& InputAction);
	void Turn(const FInputActionValue& InputAction);
	void MoveRight(const FInputActionValue& InputAction);
	void MoveForward(const FInputActionValue& InputAction);

	void JumpFunction();
	void JumpFunctionEnd();
	void SprintStart();
	void SprintEnd();


	// Button for entering/leaving possession
	void PossessionAbilityCheck();
	// Button for character's ability
	void Button1Action();
	// Additional button for anything else
	void Button2Action();


	void DashMovement(int PositiveNegativeDirection);
	void DashMovementEnd();

	// Input Actions //

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TSoftObjectPtr<UInputMappingContext> InputMapping;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* LookUpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* TurnAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* MoveRightAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* MoveForwardAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* SprintAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* PossessionAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* Ability1Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* Ability2Action;
};
