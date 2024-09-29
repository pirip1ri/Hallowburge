// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HallowburgePlayerCharacter.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "HallowburgePlayerCharacter_Ghoul.generated.h"

/**
 * 
 */
UCLASS()
class HALLOWBURGE_API AHallowburgePlayerCharacter_Ghoul : public AHallowburgePlayerCharacter
{
	GENERATED_BODY()

public:
	virtual void JumpFunction() override;
	
protected:



};
