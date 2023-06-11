// Copyright Epic Games, Inc. All Rights Reserved.

#include "BatteryCollectorGameMode.h"
#include "BatteryCollectorCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "BatteryCollectorCharacter.h"
ABatteryCollectorGameMode::ABatteryCollectorGameMode()
{
	//Enable this otherwise it wont work
	this->PrimaryActorTick.bCanEverTick = true;
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
	this->decay_rate = 0.01f;
}

void ABatteryCollectorGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	ABatteryCollectorCharacter* main_player = Cast<ABatteryCollectorCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (main_player)
	{
		if (main_player->get_current_power() > 0)
		{
			//1)decrease power per unit
			//2)So it will be frame rate independent
			float decrease_rate = -(decay_rate * main_player->get_initial_power() * DeltaTime);
			main_player->set_power(decrease_rate);
		}
	}
}

void ABatteryCollectorGameMode::BeginPlay()
{
	Super::BeginPlay();
}
