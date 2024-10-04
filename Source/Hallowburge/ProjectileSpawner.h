// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShootingProjectile.h"
#include "ProjectileSpawner.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class HALLOWBURGE_API UProjectileSpawner : public USceneComponent
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	UProjectileSpawner();

	void SpawnProjectile(TSubclassOf<AShootingProjectile> ShootingProjectile, float ChargeBoost);
};
