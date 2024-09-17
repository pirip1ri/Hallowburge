#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "HallowburgePlayerCharacter.generated.h"

class AHallowburgeSandboxGameModeBase;

UCLASS()
class HALLOWBURGE_API AHallowburgePlayerCharacter : public ACharacter
{
	GENERATED_BODY()
	// Objects //
	float MaxWalkSpeed = 0.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* Camera;

	AHallowburgeSandboxGameModeBase* GameModeRef;

	// Functions //
public:
	// Sets default values for this character's properties
	AHallowburgePlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
