// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnVolume.generated.h"

UCLASS()
class BATTERYCOLLECTOR_API ASpawnVolume : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASpawnVolume();
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	//Get the box component
	FORCEINLINE class UBoxComponent* get_box_component()
	{
		return box_component;
	}
	//Find a Random point within Box component
	UFUNCTION(BlueprintPure, Category = "Spawning")
		FVector get_random_point();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// What to spawn
	UPROPERTY(EditAnywhere, Category = "Spawning")
		TSubclassOf<class ABatteryPickup> what_to_spawn;
	FTimerHandle spawn_timer;
	//Minimum spawn delay
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
		float min_spawn_delay;
	//Maximum spawn delay
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
		float max_spawn_delay;
private:
	//Component to specific where to spawn
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning", meta = (AllowPrivateAccess = "true/false"))
		class UBoxComponent* box_component;
	//Handle spawning a pick up
	void spawn_pickup();
	float current_spawn_delay;
};
