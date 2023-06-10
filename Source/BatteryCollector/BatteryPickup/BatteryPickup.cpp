// Fill out your copyright notice in the Description page of Project Settings.


#include "BatteryPickup.h"

ABatteryPickup::ABatteryPickup()
{
	PrimaryActorTick.bCanEverTick = true;
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
}

void ABatteryPickup::Tick(float DeltaTime)
{
}

void ABatteryPickup::BeginPlay()
{
}
