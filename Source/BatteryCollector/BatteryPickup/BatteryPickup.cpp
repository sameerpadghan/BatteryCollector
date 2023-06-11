// Fill out your copyright notice in the Description page of Project Settings.


#include "BatteryPickup.h"
#include "Components/SceneComponent.h"

ABatteryPickup::ABatteryPickup()
{
	PrimaryActorTick.bCanEverTick = true;
	//GetMesh()->SetupAttachment(RootComponent);
	scence_component = CreateDefaultSubobject<USceneComponent>(TEXT("ScenceComponent"));
	RootComponent = scence_component;
	GetMesh()->SetupAttachment(RootComponent);
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	static auto mesh_name = TEXT("/Game/ExampleContent/Blueprint_Communication/Meshes/SM_Battery_Medium");
	static auto mesh_finder = ConstructorHelpers::FObjectFinder<UStaticMesh>(mesh_name);
	if (mesh_finder.Succeeded())
	{
		GetMesh()->SetStaticMesh(mesh_finder.Object);
		GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
		GetMesh()->SetWorldScale3D(FVector(1.f));
	}
	battery_power = 150.0f;
}

void ABatteryPickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABatteryPickup::was_collected_Implementation()
{
	Super::was_collected_Implementation();
	//Destroy the battery
	bool is_destroyed = this->Destroy();
	is_destroyed = Super::Destroy();
}


void ABatteryPickup::BeginPlay()
{
	Super::BeginPlay();
}

