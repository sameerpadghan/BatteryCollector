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
	FORCEINLINE float get_battery_power()
	{
		return battery_power;
	}
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Power", Meta = (BlueprintProtected = true))
		float battery_power;
private:
	UPROPERTY()
		class USceneComponent* scence_component;
};
