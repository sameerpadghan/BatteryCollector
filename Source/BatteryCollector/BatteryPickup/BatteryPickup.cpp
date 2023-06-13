// Fill out your copyright notice in the Description page of Project Settings.


#include "BatteryPickup.h"
#include "Components/SceneComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "UObject/ConstructorHelpers.h"
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
	//ParticleSystem'/Game/ExampleContent/Effects/ParticleSystems/P_electricity_arc.P_electricity_arc'
	static auto particle_system_name = TEXT("/Game/ExampleContent/Effects/ParticleSystems/P_electricity_arc");
	static auto particle_system_finder = ConstructorHelpers::FObjectFinder<UParticleSystem>(particle_system_name);
	if (particle_system_finder.Succeeded())
	{
		emitter_template = particle_system_finder.Object;
	}

}

void ABatteryPickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABatteryPickup::was_collected_Implementation()
{
	Super::was_collected_Implementation();
	FVector vector = this->GetActorLocation();
	FRotator rotator = this->GetActorRotation();
	//Destroy the battery
	bool is_destroyed = this->Destroy();
	/*
	UParticleSystemComponent* particle_component = UGameplayStatics::SpawnEmitterAttached(
		emitter_template,
		this->scence_component,
		NAME_None,
		vector,
		rotator,
		EAttachLocation::KeepRelativeOffset
	);
	*/
	UParticleSystemComponent* particle_component = UGameplayStatics::SpawnEmitterAttached(
		emitter_template,
		this->GetMesh(),
		FName("Head"),
		FVector(0, 0, 0),
		FRotator(0, 0, 0),
		EAttachLocation::KeepRelativeOffset,
		true
	);
}


void ABatteryPickup::BeginPlay()
{
	Super::BeginPlay();
}

