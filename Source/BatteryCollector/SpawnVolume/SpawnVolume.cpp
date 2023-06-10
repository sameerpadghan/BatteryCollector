// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnVolume.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "BatteryCollector/BatteryPickup/BatteryPickup.h"
// Sets default values
ASpawnVolume::ASpawnVolume()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	box_component = CreateDefaultSubobject<UBoxComponent>("BoxComponent");
	RootComponent = box_component;
	min_spawn_delay = 1.0f;
	max_spawn_delay = 5.0f;
	what_to_spawn = ABatteryPickup::StaticClass();
}

FVector ASpawnVolume::get_random_point()
{
	FVector spawn_origin = box_component->Bounds.Origin;
	FVector spawn_extent = box_component->Bounds.BoxExtent;
	FVector random_point = UKismetMathLibrary::RandomPointInBoundingBox(spawn_origin, spawn_extent);
	return random_point;
}

// Called when the game starts or when spawned
void ASpawnVolume::BeginPlay()
{
	Super::BeginPlay();
	current_spawn_delay = FMath::FRandRange(min_spawn_delay, max_spawn_delay);
	GetWorldTimerManager().SetTimer(
		spawn_timer,
		this,
		&ASpawnVolume::spawn_pickup,
		current_spawn_delay,
		false
	);
}

void ASpawnVolume::spawn_pickup()
{
	if (what_to_spawn != NULL)
	{
		auto world = GetWorld();
		if (world)
		{
			//Setting spawn parameter
			FActorSpawnParameters spawn_params;
			spawn_params.Owner = this;
			spawn_params.Instigator = this->GetInstigator();
			//getting random location
			FVector spawn_location = get_random_point();
			//getting a random rotation
			FRotator spawn_rotation;
			spawn_rotation.Yaw = FMath::FRand() * 360.0f;
			spawn_rotation.Pitch = FMath::FRand() * 360.0f;
			spawn_rotation.Roll = FMath::FRand() * 360.0f;
			//spawn the pickup
			world->SpawnActor<ABatteryPickup>(
				what_to_spawn,
				spawn_location,
				spawn_rotation,
				spawn_params
			);
			current_spawn_delay = FMath::FRandRange(min_spawn_delay, max_spawn_delay);
			GetWorldTimerManager().SetTimer(
				spawn_timer,
				this,
				&ASpawnVolume::spawn_pickup,
				current_spawn_delay,
				false
			);
		}
	}
}

// Called every frame
void ASpawnVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

