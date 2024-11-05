

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PossessableCharacter.h"
#include "GameFramework/PlayerController.h"
#include "GhostPlayerCharacter.generated.h"

UENUM()
enum class EPunchState : uint8
{
	Idle			UMETA(DisplayName = "Idle"),
	Charging        UMETA(DisplayName = "Charging"),
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

protected:
	bool bCanPossess = true;
	FTimerHandle PossessionProgressTimerHandle;
	FVector StartPossessionLocation;

	UPROPERTY(BlueprintReadOnly)
	EPunchState PunchState = EPunchState::Idle;

	float PunchChargeTime;


	// Functions //

	// Sets default values for this character's properties
	AGhostPlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PossessiveDashStart() override;
	virtual void ActionButton1() override;
	virtual void ActionButton1End() override;


	UFUNCTION(BlueprintCallable, Category = "Ghost")
	virtual void PossessCharacterCheck(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	UFUNCTION(BlueprintCallable, Category = "Ghost")
	void ChangeControlledCharacter(AHallowburgePlayerController* PlayerController, APossessableCharacter* PossessedCharacter);
	
	
	void PossessiveDashEnd();
	
	void Punch();
	void AirPunch();
	void ChargedPunch();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
