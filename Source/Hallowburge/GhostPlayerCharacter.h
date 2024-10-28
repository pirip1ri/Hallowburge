

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PossessableCharacter.h"
#include "GameFramework/PlayerController.h"
#include "GhostPlayerCharacter.generated.h"

// Enum that checks what the player can do in any specific state

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

	// Functions //

	// Sets default values for this character's properties
	AGhostPlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PossessionAbilityCheck() override;
	UFUNCTION(BlueprintCallable, Category = "Ghost")
	virtual void PossessEnemyCharacter(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	void EndPossessionCheck();
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
