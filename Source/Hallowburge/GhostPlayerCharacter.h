// words

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PossessableCharacter.h"
#include "Components/BoxComponent.h"
#include "AIController.h"
#include "GameFramework/PlayerController.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "GhostPlayerCharacter.generated.h"

UENUM(BlueprintType)
enum class EPunchState : uint8
{
	Idle			UMETA(DisplayName = "Idle"),
	Charging        UMETA(DisplayName = "Charging"),
	Punch			UMETA(DisplayName = "Punch"),
	AirAttack		UMETA(DisplayName = "AirAttack"),
	SpinAttack		UMETA(DisplayName = "SpinAttack"),
	Cooldown		UMETA(DisplayName = "Cooldown")
};

UCLASS()
class HALLOWBURGE_API AGhostPlayerCharacter : public APossessableCharacter
{
	GENERATED_BODY()

	// Objects // 
public:
	UPROPERTY()
	TWeakObjectPtr<AAIController> AIControllerStash;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Collision")
	UCapsuleComponent* PossessionCapsule;

	UPROPERTY(BlueprintReadWrite, Category = "Input")
	bool bIsHoldingDownPunchButton = false;

protected:
	bool bCanPossess = true;
	FVector StartPossessionLocation;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Collision")
	UBoxComponent* RightHandCollisionBox;


	UPROPERTY(BlueprintReadWrite, Category = "Abilities")
	EPunchState PunchState = EPunchState::Idle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	float PunchCooldown;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float BaseDamage = 2.0f;  // Base damage to apply
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float RadialForceStrength = 100.0f;  // Strength of the radial force to apply
	UPROPERTY(VisibleAnywhere, Category = "Damage")
	URadialForceComponent* RadialForceComponentFist;
	UPROPERTY(VisibleAnywhere, Category = "Damage")
	URadialForceComponent* RadialForceComponentCharge;

	// Functions //

	// Sets default values for this character's properties
	AGhostPlayerCharacter();
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PossessiveDashCall() override;
	virtual void ActionButton1() override;
	virtual void ActionButton1End() override;


	UFUNCTION(BlueprintCallable, Category = "Ghost")
	virtual void OnPossessionOrSpecialPunchOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	UFUNCTION(BlueprintCallable, Category = "Ghost")
	void ChangeControlledCharacter(AHallowburgePlayerController* PlayerController, APossessableCharacter* PossessedCharacter);
	UFUNCTION(BlueprintCallable, Category = "Ghost")
	void PossessiveDashEnd();
	
	void Punch();
	void AirPunch();
	void ChargedPunch();
	UFUNCTION(BlueprintCallable, Category = "Ghost")
	void OnPunchOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION(BlueprintCallable, Category = "Ghost")
	void EndPunch();

	UFUNCTION(BlueprintNativeEvent, Category = "Ghost")
	void PlayPunchMontage(EPunchState CurrentPunchState);

	UFUNCTION(BlueprintNativeEvent, Category = "Ghost")
	void PlayDashMontage();

	UFUNCTION(BlueprintCallable, Category = "Ghost")
	void EnablePossessionFunctionality();

	UFUNCTION()
	void DamageOtherActor(AActor* OtherActor, UPrimitiveComponent* OverlappedComp);
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintNativeEvent, Category = "Ghost")
	void PlayUnpossessMontage();
};
