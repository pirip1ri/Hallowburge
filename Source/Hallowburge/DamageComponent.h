// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DamageComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HALLOWBURGE_API UDamageComponent : public UActorComponent
{
	GENERATED_BODY()

public:
    UDamageComponent();

    UFUNCTION(BlueprintCallable, Category = "Damage")
    void ApplyDamage(class APossessableCharacter* Target);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage", meta = (AllowPrivateAccess = "true"))
    float DamageAmount = 1.0f;

protected:
    virtual void BeginPlay() override;

};
