// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "Assignment_ProjectCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "MovingPlatform.h"
#include "kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Robot.h"
#include "UnrealMathUtility.h"
#include <DrawDebugHelpers.h>
#include "Private/Automatic_Gun.h"

//////////////////////////////////////////////////////////////////////////
// AAssignment_ProjectCharacter

AAssignment_ProjectCharacter::AAssignment_ProjectCharacter()
{
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
	HastheItem = false;
	canPickupItem = false;
	OverallExposionTime = 20.f;

	
}

//////////////////////////////////////////////////////////////////////////
// Input

void AAssignment_ProjectCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &AAssignment_ProjectCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AAssignment_ProjectCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AAssignment_ProjectCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AAssignment_ProjectCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AAssignment_ProjectCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AAssignment_ProjectCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AAssignment_ProjectCharacter::OnResetVR);

	PlayerInputComponent->BindAction("PickUp", IE_Pressed, this, &AAssignment_ProjectCharacter::PickupItem);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AAssignment_ProjectCharacter::fire);
	TimerStatus = false;
	TimeLeft = OverallExposionTime;
	CurrentHealth = 100;
}


void AAssignment_ProjectCharacter::BeginPlay()
{
	Super::BeginPlay();
	ShowObjectiveText(TEXT("Objective : Steal the Hard Drive"), FColor::Yellow);
}

void AAssignment_ProjectCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (TimerStatus) {
		TimeLeft -= DeltaTime;


	}
}

void AAssignment_ProjectCharacter::startExplosionCountDown()
{
	TimerStatus = true;
	//timer 
	GetWorld()->GetTimerManager().SetTimer(ExplosionTimer, this, &AAssignment_ProjectCharacter::BigExplosion, OverallExposionTime, false);
	ShowTimerProgressBar();
}

void AAssignment_ProjectCharacter::PickupItem()
{
	if (canPickupItem && Availableitem)
	{
		Availableitem->Destroy();
		startExplosionCountDown();
		ShowObjectiveText(TEXT("Self destruction System Activated ! Evacuate The Area Before Explosion") ,FColor::Red);
		TArray<AActor*> foundMovingPlatformsactors;
		TArray<AActor*> foundRobots;
		TArray<AActor*> Found_Automatic_Guns;
		UGameplayStatics::GetAllActorsOfClass(this, MovingPlatformclass, foundMovingPlatformsactors);
		UGameplayStatics::GetAllActorsOfClass(this, RobotClass, foundRobots);
		UGameplayStatics::GetAllActorsOfClass(this, AutomaticGunClass, Found_Automatic_Guns);
		for (AActor* aa : foundMovingPlatformsactors) {
			AMovingPlatform* mp = Cast<AMovingPlatform>(aa);
			if (mp)
			{
				mp->ActivateMovement = true;
			}


		}
		for (AActor* bb : foundRobots) {
			ARobot* rob = Cast<ARobot>(bb);
			if (rob)
			{
				rob->Attacking = true;
			}
		}
		for (AActor* gg : Found_Automatic_Guns) {
			AAutomatic_Gun* A_gun = Cast<AAutomatic_Gun>(gg);
			if (A_gun)
			{
				A_gun->Activate();
			}
		}
	}
}

void AAssignment_ProjectCharacter::BigExplosion()
{
	if (IsInSafeArea == false) {
		APlayerController* pc = Cast<APlayerController>(this->GetController());
		if (pc)
		{
			this->DisableInput(pc);
			LostTheGame();
		}
	}
	
}







void AAssignment_ProjectCharacter::WinTheGame()
{
	ShowObjectiveText(TEXT("Successfully evacuated the area Before Explosion , You Won"),FColor::Green);
}

void AAssignment_ProjectCharacter::LostTheGame()
{
	ShowObjectiveText(TEXT("Game over!! You died"), FColor::Red);
}

void AAssignment_ProjectCharacter::fire()
{
	FHitResult hitr;
	FCollisionQueryParams qparams;
	qparams.AddIgnoredActor(this);
	if (GetWorld()->LineTraceSingleByChannel(hitr, FollowCamera->GetComponentLocation(), FollowCamera->GetComponentLocation() + FollowCamera->GetForwardVector() * 3000, ECollisionChannel::ECC_Visibility,
		qparams))
	{
		
		ARobot* robotref = Cast<ARobot>(hitr.GetActor());
		if (robotref)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), RobotHitParticle, hitr.ImpactPoint, FRotator::ZeroRotator ,FVector(3.0f));
			robotref->RecieveDamage(20);
		}
	}
	//GetMesh->GetSocketLocation(TEXT("hand_r"))
	DrawDebugLine(GetWorld(), FollowCamera->GetComponentLocation() + FollowCamera->GetForwardVector()*CameraBoom->TargetArmLength, (FollowCamera->GetComponentLocation() + FollowCamera->GetForwardVector() * 3000) , FColor::Blue, false, 1.0f, 0.0f, 3.0f );
}

void AAssignment_ProjectCharacter::RecieveDamage(float DamageAmount)
{
	CurrentHealth = FMath::Clamp(CurrentHealth - DamageAmount, 0.0f, 100.0f);
	DamageRecieve_UI(CurrentHealth);
	if (CurrentHealth<=0.0f)
	{
		//GetMesh()->SetSimulatePhysics(true);
		BigExplosion();
	}
}

void AAssignment_ProjectCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AAssignment_ProjectCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void AAssignment_ProjectCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void AAssignment_ProjectCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AAssignment_ProjectCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AAssignment_ProjectCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AAssignment_ProjectCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
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
