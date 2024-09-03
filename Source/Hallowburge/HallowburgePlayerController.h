// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "InputActionValue.h"
#include "GameFramework/PlayerController.h"
#include "HallowburgePlayerController.generated.h"

// Enum that checks what the player can do in any specific state
UENUM(BlueprintType)
enum class EPlayerCharacterController : uint8
{
	None		UMETA(DisplayName = "None"),
	Ghoul		UMETA(DisplayName = "Ghoul"),
	Astronaut	UMETA(DisplayName = "Astronaut")
};

UCLASS()
class HALLOWBURGE_API AHallowburgePlayerController : public APlayerController
{
	GENERATED_BODY()
	
	// Universal Parameters //
protected:
	EPlayerCharacterController PlayerCharacterController;

	// Universal Functions //
protected:
	void SetupInputComponent() override;

	void LookUp(const FInputActionValue& InputAction);
	void Turn(const FInputActionValue& InputAction);
	void MoveRight(const FInputActionValue& InputAction);
	void MoveForward(const FInputActionValue& InputAction);

	void JumpAction();
	void JumpActionEnded();
	void CrouchStart();
	void CrouchEnd();

	// Button for entering/leaving possession
	void Button1Action();
	// Button for character's ability
	void Button2Action();
	// Additional button for anything else
	void Button3Action();
};
