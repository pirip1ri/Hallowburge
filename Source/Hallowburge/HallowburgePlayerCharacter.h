#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PossessableCharacter.h"
#include "GameFramework/PlayerController.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "HallowburgePlayerCharacter.generated.h"

// Enum that checks what the player can do in any specific state
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
class HALLOWBURGE_API AHallowburgePlayerCharacter : public APossessableCharacter
{
	GENERATED_BODY()



	// Objects // 

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* Camera;

	AHallowburgeSandboxGameModeBase* GameModeRef;

	// Objects - Animation stuff //

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float CharacterSpeed;
	float MaxWalkSpeed = 600.0f;

public:
	EPlayerPawn PlayerPawn;
	virtual void JumpFunction() override;




	// Functions //

	// Sets default values for this character's properties
	AHallowburgePlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// When the character lands - MIGHT NEED TO MOVE
	//virtual void Landed(const FHitResult& Hit) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void PossessEnemy(ACharacter* TargetEnemy);
	void UnpossessEnemy();
};
