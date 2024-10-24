#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputSubsystems.h"
#include "PossessableCharacter.h"
#include "HallowburgeSandboxGameModeBase.h"
#include "GameFramework/PlayerController.h"
#include "HallowburgePlayerController.generated.h"

UENUM(BlueprintType)
enum class EPlayersCurrentPlayState : uint8
{
	None	UMETA(DisplayName = "None"),
	Playing	UMETA(DisplayName = "Playing"),
	InMenus	UMETA(DisplayName = "InMenus"),
	Dead	UMETA(DisplayName = "Dead")
};


UCLASS()
class HALLOWBURGE_API AHallowburgePlayerController : public APlayerController
{
	GENERATED_BODY()

	
	// Universal Parameters //
public:

	AHallowburgeSandboxGameModeBase* GameModeRef;
	APossessableCharacter* PlayerCharacter = Cast<APossessableCharacter>(GetCharacter());

	UPROPERTY()
	EPlayersCurrentPlayState PlayersCurrentPlayState = EPlayersCurrentPlayState::None;

	UEnhancedInputLocalPlayerSubsystem* EnhancedInputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());

protected:

	float RunSpeed = 600.0f;
	float SprintSpeed = 900.0f;
	float MaxWalkSpeed;

	/** Base lookup rate, in deg/sec. Other scaling may affect final lookup rate. */
	UPROPERTY(EditAnywhere, Category = "Look")
	float BaseLookUpRate = 90.0f;

	/** Base lookright rate, in deg/sec. Other scaling may affect final lookup rate. */
	UPROPERTY(EditAnywhere, Category = "Look")
	float BaseLookRightRate = 90.0f;



	// Universal Functions //
public:
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintNativeEvent, Category = "Input")
	void SwitchInputMappingContext(APossessableCharacter* NewPossessedCharacter, int Priority);


	// Player Input Function Calls //
protected:

	void SetupInputComponent() override;

	UFUNCTION(BlueprintCallable, Category = "Input")
	void LookUp(const float& InputValue);
	UFUNCTION(BlueprintCallable, Category = "Input")
	void Turn(const float& InputValue);
	UFUNCTION(BlueprintCallable, Category = "Input")
	void MoveRight(const float& InputValue);
	UFUNCTION(BlueprintCallable, Category = "Input")
	void MoveForward(const float& InputValue);

	UFUNCTION(BlueprintCallable, Category = "Input")
	void JumpFunction();
	UFUNCTION(BlueprintCallable, Category = "Input")
	void JumpFunctionEnd();
	UFUNCTION(BlueprintCallable, Category = "Input")
	void SprintStart();
	UFUNCTION(BlueprintCallable, Category = "Input")
	void SprintEnd();


	UFUNCTION(BlueprintCallable, Category = "Input")
	// Button for entering/leaving possession
	void PossessionAbilityCheck();
	UFUNCTION(BlueprintCallable, Category = "Input")
	// Button for character's ability
	void Button1Action();
	UFUNCTION(BlueprintCallable, Category = "Input")
	// Additional button for anything else
	void Button2Action();
	UFUNCTION(BlueprintCallable, Category = "Input")
	void Button1ActionEnd();
	UFUNCTION(BlueprintCallable, Category = "Input")
	void Button2ActionEnd();

	UFUNCTION(BlueprintCallable, Category = "Player")
	EPlayersCurrentPlayState GetPlayersCurrentPlayState();

	UFUNCTION(BlueprintCallable, Category = "Player")
	void SetPlayersCurrentPlayState(EPlayersCurrentPlayState PlayersPlayState);
};
