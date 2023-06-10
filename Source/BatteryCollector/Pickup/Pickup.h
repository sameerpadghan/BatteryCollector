// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pickup.generated.h"

UCLASS()
class BATTERYCOLLECTOR_API APickup : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APickup();
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	//return mesh for this class
	FORCEINLINE class UStaticMeshComponent* GetMesh() const
	{
		return mesh_component;
	}
	UFUNCTION(BlueprintPure, Category = "Pickup")
		bool is_active();
	//
	UFUNCTION(BlueprintCallable, Category = "Pickup")
		void set_active(bool new_pick_up_state);
	//Was the pickup collected
	UFUNCTION(BlueprintNativeEvent)
		void was_collected();
	//1)Need to be override by child class
	//2)But always call it as was_Collected() 
	virtual void was_collected_Implementation();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	//1)True when the pick up is active
	bool b_isactive;
private:
	//Forward declare Mesh Component
	//Otherwise it gives error
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pickup", meta = (AllowPrivateAccess = "true/false"))
		class UStaticMeshComponent* mesh_component;
};
