// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BatteryCollectorCharacter.generated.h"

UCLASS(config = Game)
class ABatteryCollectorCharacter : public ACharacter
{
	GENERATED_BODY()
private:

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FollowCamera;
	//Collection Sphere Component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USphereComponent* collection_sphere_component;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Power", meta = (AllowPrivateAccess = "true"))
		float current_power;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Power", meta = (AllowPrivateAccess = "true"))
		int collected_battery;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Power", meta = (AllowPrivateAccess = "true"))
		float collected_power;
	/*Text above player head*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Power", meta = (AllowPrivateAccess = "true"))
		class UTextRenderComponent* text_component;
	//Power Material
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PowerMaterial", meta = (AllowPrivateAccess = "true"))
		class UMaterialInstanceDynamic* power_material;
	//Power Material
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PowerMaterial", meta = (AllowPrivateAccess = "true"))
		class UMaterialInterface* material;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PowerMaterial", meta = (AllowPrivateAccess = "true"))
		class UMaterialInstanceDynamic* dynamic_material;
public:
	ABatteryCollectorCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate;

protected:

	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface
	// called when pressed a key to collect pick up inside the sphere
	UFUNCTION(BlueprintCallable, Category = "Pickup")
		void collect_pickup();
	/*Initial power of the chracter*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Power")
		float initial_power;
	//Speed when power level = 0
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Power", Meta = (BlueprintProtected = "true"))
		float base_speed;
	//Multiplyer for character speed
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Power", Meta = (BlueprintProtected = "true"))
		float speed_factor;

	UFUNCTION()
		void power_effect();
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const
	{
		return CameraBoom;
	}
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const
	{
		return FollowCamera;
	}
	/* Return the Collection Box Sphere*/
	FORCEINLINE class USphereComponent* get_collection_sphere_component() const
	{
		return collection_sphere_component;
	}
	UFUNCTION()
		FORCEINLINE float get_current_power()
	{
		return current_power;
	}
	UFUNCTION()
		FORCEINLINE float get_initial_power()
	{
		return initial_power;
	}
	/*
	* 1)Function to update player power
	* 2)where power is the param to give to set the power
	*/
	UFUNCTION()
		void set_power(float power);

	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void Tick(float DeltaTime) override;
};

