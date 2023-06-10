// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BatteryCollector/Pickup/Pickup.h"
#include "BatteryPickup.generated.h"

/**
 *
 */
UCLASS()
class BATTERYCOLLECTOR_API ABatteryPickup : public APickup
{
	GENERATED_BODY()
public:
	ABatteryPickup();
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void was_collected_Implementation() override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
private:
	UPROPERTY()
		class USceneComponent* scence_component;
};
