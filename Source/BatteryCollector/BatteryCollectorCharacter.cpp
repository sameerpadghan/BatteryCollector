// Copyright Epic Games, Inc. All Rights Reserved.

#include "BatteryCollectorCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SphereComponent.h"
#include "BatteryPickup/BatteryPickup.h"
#include "Components/TextRenderComponent.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "UObject/UObjectGlobals.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "UObject/UObjectGlobals.h"

//////////////////////////////////////////////////////////////////////////
// ABatteryCollectorCharacter

ABatteryCollectorCharacter::ABatteryCollectorCharacter()
{
	this->PrimaryActorTick.bCanEverTick = true;
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
	collection_sphere_component = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	collection_sphere_component->SetupAttachment(RootComponent);
	collection_sphere_component->SetSphereRadius(200.0f);
	initial_power = 2000.0f;
	current_power = initial_power;
	collected_power = 0.0f;
	text_component = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Text"));
	text_component->SetupAttachment(RootComponent);
	text_component->HorizontalAlignment = EHorizTextAligment::EHTA_Center;
	text_component->VerticalAlignment = EVerticalTextAligment::EVRTA_TextCenter;
	static FText f_text_number = FText::AsNumber(current_power);
	text_component->Text = f_text_number;
	speed_factor = 0.75f;
	base_speed = 10.0f;
}

//////////////////////////////////////////////////////////////////////////
// Input

void ABatteryCollectorCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("Collect", IE_Pressed, this, &ABatteryCollectorCharacter::collect_pickup);
	////////////
	PlayerInputComponent->BindAxis("MoveForward", this, &ABatteryCollectorCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ABatteryCollectorCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ABatteryCollectorCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ABatteryCollectorCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &ABatteryCollectorCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &ABatteryCollectorCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &ABatteryCollectorCharacter::OnResetVR);
}

void ABatteryCollectorCharacter::collect_pickup()
{
	//Get all over-lapping actor and store them in array
	TArray<AActor*> collected_actor;
	collection_sphere_component->GetOverlappingActors(collected_actor);
	//For each actor we collected
	for (int32 collected = 0; collected < collected_actor.Num(); ++collected)
	{
		//Cast the actor to pick up
		ABatteryPickup* test_pickup = Cast<ABatteryPickup>(collected_actor[collected]);
		if (test_pickup && !test_pickup->IsPendingKill() && test_pickup->is_active())
		{
			test_pickup->was_collected();
			test_pickup->set_active(false);
			//Increase battery count
			collected_battery++;
			//Increase collected power
			collected_power = collected_power + test_pickup->get_battery_power();
		}
	}
	if (collected_power > 0)
	{
		this->set_power(collected_power);
	}
}

void ABatteryCollectorCharacter::power_effect()
{
	float result = get_current_power() / get_initial_power();
	static float clamp_result = FMath::Clamp<float>(result, 0.0f, 1.0f);
	FLinearColor black = FLinearColor(0, 0, 0);
	FLinearColor white = FLinearColor(255, 255, 255);
	FLinearColor result_color = UKismetMathLibrary::LinearColorLerp(black, white, clamp_result);
	//1)Name of the parameter is BodyColor
	//2)Take from the video
	FName parameter_name = FName(TEXT("BodyColor"));
	if (power_material != NULL)
	{
		this->power_material->SetVectorParameterValue(parameter_name, result_color);
	}
}

void ABatteryCollectorCharacter::BeginPlay()
{
	Super::BeginPlay();

}

void ABatteryCollectorCharacter::set_power(float power)
{
	//Change power
	this->current_power = this->current_power + power;
	//Change speed
	GetCharacterMovement()->MaxWalkSpeed = base_speed + (speed_factor * current_power);
	power_effect();
	FString string = FString::SanitizeFloat(current_power);
	//1)Below function is deprecated
	//2)Remove it if program is not compiling
	text_component->SetText(string);
}

void ABatteryCollectorCharacter::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	//1)Do this in the begin play and not in the constructor
	//2)Because the  we have not set Mesh in the constructor
	//3)Mesh it set using blueprint
	//First element is zero
	material = this->GetMesh()->GetMaterial(0);
	if (material != NULL)
	{
		//power_material = UKismetMaterialLibrary::CreateDynamicMaterialInstance(this, material, NAME_None, EMIDCreationFlags::Transient);
		power_material = NewObject<UMaterialInstanceDynamic>();
		power_material->ClearFlags(EObjectFlags::RF_MarkAsNative);
		power_material->AddToRoot();
		power_material->Create(material, this);
	}
}

void ABatteryCollectorCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void ABatteryCollectorCharacter::OnResetVR()
{
	// If BatteryCollector is added to a project via 'Add Feature' in the Unreal Editor the dependency on HeadMountedDisplay in BatteryCollector.Build.cs is not automatically propagated
	// and a linker error will result.
	// You will need to either:
	//		Add "HeadMountedDisplay" to [YourProject].Build.cs PublicDependencyModuleNames in order to build successfully (appropriate if supporting VR).
	// or:
	//		Comment or delete the call to ResetOrientationAndPosition below (appropriate if not supporting VR)
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void ABatteryCollectorCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	Jump();
}

void ABatteryCollectorCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	StopJumping();
}

void ABatteryCollectorCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ABatteryCollectorCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ABatteryCollectorCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ABatteryCollectorCharacter::MoveRight(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}
