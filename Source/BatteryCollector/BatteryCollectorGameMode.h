// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BatteryCollectorGameMode.generated.h"

//enum to store the cuurent  state of gameplay
UENUM(BlueprintType)
enum class EBatteryPlayState : uint8
{
	EPlaying,
	EGameOver,
	EWon,
	EUnknow
};





UCLASS(minimalapi)
class ABatteryCollectorGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ABatteryCollectorGameMode();
	virtual void Tick(float DeltaTime) override;
	//return the power to win
	UFUNCTION(BlueprintCallable)
		float get_power_to_win() const;
	//Get the current state
	UFUNCTION()
		EBatteryPlayState get_current_state() const;
	//Set the current state
	UFUNCTION()
		void set_current_state(EBatteryPlayState state);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Rate at which Character loses power
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Power", Meta = (BlueprintProtected = "true"))
		float decay_rate;

	//Power needed to win the game
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Power", Meta = (BlueprintProtected = "true"))
		float power_to_win;

	//Widget class to use for HUD screen(Template class)
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Power", Meta = (BlueprintProtected = "true"))
		TSubclassOf<class UUserWidget> HUDWidgetClass;

	//The actual instance of the HUD screen(instance of the above class)
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Power", Meta = (BlueprintProtected = "true"))
		class UUserWidget* current_widget;
private:
	//Current state of the game
	EBatteryPlayState current_state;

};



