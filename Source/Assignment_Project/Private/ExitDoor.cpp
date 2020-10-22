// Fill out your copyright notice in the Description page of Project Settings.


#include "ExitDoor.h"
#include <Engine/Engine.h>
#include "MAth/UnrealMathUtility.h"

// Sets default values
AExitDoor::AExitDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	doorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Doormesh"));
	RootComponent = doorMesh;
	bISactive = false;
	DoorSpeed = 5.f;
}

// Called when the game starts or when spawned
void AExitDoor::BeginPlay()
{
	Super::BeginPlay();
	InitialLocation = doorMesh->RelativeLocation;
}

// Called every frame
void AExitDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bISactive && doorMesh->RelativeLocation!=InitialLocation+DoorRelativeOffset)
	{
		doorMesh->SetRelativeLocation(FMath::VInterpTo(doorMesh->RelativeLocation, InitialLocation + DoorRelativeOffset, DeltaTime, DoorSpeed));
	}
}



