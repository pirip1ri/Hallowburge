#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "InputActionValue.h"
#include "EnhancedInputSubsystems.h"
#include "HallowburgePlayerCharacter.h"
#include "HallowburgeSandboxGameModeBase.h"
#include "GameFramework/PlayerController.h"
#include "HallowburgePlayerController.generated.h"

// Enum that checks what the player can do in any specific state
UENUM(BlueprintType)
enum class EPlayerPawnController : uint8
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
public:
	AHallowburgeSandboxGameModeBase* GameModeRef;

protected:
	EPlayerPawnController* PlayerCharacterController;
	AHallowburgePlayerCharacter* PlayerCharacter;


	/** Base lookup rate, in deg/sec. Other scaling may affect final lookup rate. */
	UPROPERTY(EditAnywhere, Category = "Look")
	float BaseLookUpRate = 90.0f;

	/** Base lookright rate, in deg/sec. Other scaling may affect final lookup rate. */
	UPROPERTY(EditAnywhere, Category = "Look")
	float BaseLookRightRate = 90.0f;

	float MaxWalkSpeed;




	// Universal Functions //
public:
	virtual void BeginPlay() override;

protected:
	void SetupInputComponent() override;

	void LookUp(const FInputActionValue& InputAction);
	void Turn(const FInputActionValue& InputAction);
	void MoveRight(const FInputActionValue& InputAction);
	void MoveForward(const FInputActionValue& InputAction);

	void JumpFunction();
	void JumpFunctionEnded();
	void CrouchStart();
	void CrouchEnd();

	// Button for entering/leaving possession
	void PossessionFunction();
	// Button for character's ability
	void Button1Action();
	// Additional button for anything else
	void Button2Action();

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
	UInputAction* CrouchAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* PossessionAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* Ability1Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* Ability2Action;
};
