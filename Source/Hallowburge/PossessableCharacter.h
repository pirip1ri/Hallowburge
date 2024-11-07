// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "PossessableCharacter.generated.h"


UENUM(BlueprintType)
enum class EPlayerPawn : uint8
{
	None				UMETA(DisplayName = "None"),
	Ghost				UMETA(DisplayName = "Ghost"),
	PossessedCreature	UMETA(DisplayName = "Possessed Creature")
};

class AHallowburgeSandboxGameModeBase;
class AGhostPlayerCharacter;
class AAHallowburgePlayerController;

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
	
public:
	AGhostPlayerCharacter* GhostCharacter = nullptr;


	// Objects - Animation stuff //

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float CharacterSpeed;
	float MaxWalkSpeed = 600.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bCanDash;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dash")
	float DashCooldown;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dash")
	float DashDistance = 1800.0f;
	FTimerHandle DashCooldownTimerHandle;

	// For Animations
	UPROPERTY(EditAnywhere)
	UAnimMontage* DashMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TSoftObjectPtr<UInputMappingContext> InputMapping;

	// Functions //

	// Sets default values for this character's properties
	APossessableCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable) // for nav links
	virtual void JumpFunction();
	UFUNCTION(BlueprintCallable)
	virtual void JumpFunctionEnd();
	UFUNCTION(BlueprintCallable)
	virtual void PossessiveDashStart(); // maybe add variable APossessableCharacter* OtherActor
	virtual void ActionButton1() {}
	virtual void ActionButton2() {}
	virtual void ActionButton1End() {}
	virtual void ActionButton2End() {}

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void DashMovement(float PositiveNegativeDirection);
	void DashMovementEnd();
	virtual void OnPossessionOrSpecialPunchOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult); // maybe add variable APossessableCharacter* TargetCharachter

public:
	UFUNCTION(BlueprintNativeEvent, Category = "Health")
	void OnDeath();

	UFUNCTION(BlueprintNativeEvent, Category = "Possession")
	void OnPossessCharacterInScene(APossessableCharacter* PossessedCharacter, AHallowburgePlayerController* PlayerController);

	UFUNCTION(BlueprintNativeEvent, Category = "Possession")
	void OnUnpossessCharacterInScene(APossessableCharacter* PossessedCharacter);
};
