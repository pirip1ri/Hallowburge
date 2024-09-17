// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "HallowburgeSandboxGameModeBase.generated.h"

class AHallowburgePlayerCharacter;
/**
 * 
 */
UCLASS()
class HALLOWBURGE_API AHallowburgeSandboxGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AHallowburgeSandboxGameModeBase();

	virtual void BeginPlay() override;
};
