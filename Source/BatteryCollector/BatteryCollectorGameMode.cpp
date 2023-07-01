// Copyright Epic Games, Inc. All Rights Reserved.

#include "BatteryCollectorGameMode.h"
#include "BatteryCollectorCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "BatteryCollectorCharacter.h"
#include "Blueprint/UserWidget.h"
#include "SpawnVolume/SpawnVolume.h"
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
	//WidgetBlueprint'/Game/HUD/BatteryHUD'
	static auto hud_name = TEXT("/Game/HUD/BatteryHUD");
	static auto hud_finder = ConstructorHelpers::FClassFinder<UUserWidget>(hud_name);
	if (hud_finder.Succeeded())
	{
		HUDWidgetClass = hud_finder.Class;
	}
}

void ABatteryCollectorGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	ABatteryCollectorCharacter* main_player = Cast<ABatteryCollectorCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (main_player)
	{
		// if our power is greater than  needed to win set the game  state to won
		if (main_player->get_current_power() > power_to_win)
		{
			current_state = EBatteryPlayState::EGameOver;
		}
		// if the character power is positive
		else if (main_player->get_current_power() > 0)
		{
			//1)decrease power per unit
			//2)So it will be frame rate independent
			float decrease_rate = -(decay_rate * main_player->get_initial_power() * DeltaTime);
			main_player->set_power(decrease_rate);
		}
		else
		{
			current_state = EBatteryPlayState::EGameOver;
		}
	}
}

float ABatteryCollectorGameMode::get_power_to_win() const
{
	return power_to_win;
}

EBatteryPlayState ABatteryCollectorGameMode::get_current_state() const
{
	return this->current_state;
}

void ABatteryCollectorGameMode::set_current_state(EBatteryPlayState state)
{
	this->current_state = state;
}

void ABatteryCollectorGameMode::BeginPlay()
{
	Super::BeginPlay();
	this->current_state = EBatteryPlayState::EPlaying;
	ABatteryCollectorCharacter* main_player = Cast<ABatteryCollectorCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	//set the score 
	if (main_player)
	{
		power_to_win = main_player->get_initial_power() * 1.25;
	}
	if (HUDWidgetClass != nullptr)
	{
		current_widget = CreateWidget<UUserWidget>(GetWorld(), HUDWidgetClass);
		if (current_widget != nullptr)
		{
			current_widget->AddToViewport();
		}
	}
	//Find all spawn volume Actor
	TArray< AActor*> found_actors;
	UGameplayStatics::GetAllActorsOfClass(
		this,
		ASpawnVolume::StaticClass(),
		found_actors
	);
	for (auto actor : found_actors)
	{
		ASpawnVolume* spawn_volume_actor = Cast<ASpawnVolume>(actor);
		if (spawn_volume_actor)
		{
			spawn_volume_array.AddUnique(spawn_volume_actor);
		}
	}
	for (auto actor : spawn_volume_array)
	{
		actor->set_spawning_active(true);
	}
}
