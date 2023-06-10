// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickup.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
// Sets default values
APickup::APickup()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	this->b_isactive = true;
	mesh_component = CreateDefaultSubobject<UStaticMeshComponent>("MeshComponent");
	mesh_component->SetupAttachment(RootComponent);
	//1)This is a general pick up class
	//2)Inherit this to create a specific class
	//3)Example BatteryPickup,StonePickup
	/*
	static auto mesh_name = TEXT("/Game/ExampleContent/Blueprint_Communication/Meshes/SM_Battery_Medium");
	static auto mesh_finder = ConstructorHelpers::FObjectFinder<UStaticMesh>(mesh_name);
	if (mesh_finder.Succeeded())
	{
		mesh_component->SetStaticMesh(mesh_finder.Object);
		mesh_component->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
		mesh_component->SetWorldScale3D(FVector(1.f));
	}
	*/
}

bool APickup::is_active()
{
	return this->b_isactive;
}

void APickup::set_active(bool new_pick_up_state)
{
	this->b_isactive = new_pick_up_state;
}

// Called when the game starts or when spawned
void APickup::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void APickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}