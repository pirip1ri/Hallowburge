// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HALLOWBURGE_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();

	UFUNCTION(BlueprintCallable)
	void DeductHealth(class APossessableCharacter* DamagedActor, float Damage);

	UFUNCTION()
	void HandleCollisionDamage(AActor* CollidedActor, float Damage, AController* Controller, AActor* OffenseActor);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health", meta = (ClampMin = "0.0"))
	float MaxHealth;

	UPROPERTY(BlueprintReadOnly)
	float CurrentHealth;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage", meta = (ClampMin = "0"))
	int AttackTokenCount;
	UFUNCTION(BlueprintCallable, Category = "Damage")
	bool ReserveAttackToken(int Amount);
	UFUNCTION(BlueprintCallable, Category = "Damage")
	void ReturnAttackToken(int Amount);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	TSubclassOf<UDamageType> DamageTypeClass;  // Damage type (can be set in the editor)
};
