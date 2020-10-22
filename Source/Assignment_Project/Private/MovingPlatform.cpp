// Fill out your copyright notice in the Description page of Project Settings.


#include "MovingPlatform.h"
#include "Components/StaticMeshComponent.h"
// Sets default values
AMovingPlatform::AMovingPlatform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MovementDirecton = true;
	MovementRange = 600;
	movespeed = 100;
	ActivateMovement = false;
	meshcomp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("meshcomp"));
	RootComponent = meshcomp;
}

// Called when the game starts or when spawned
void AMovingPlatform::BeginPlay()
{
	Super::BeginPlay();
	Initpos = this->GetActorLocation();

}

// Called every frame
void AMovingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (ActivateMovement)
	{
		FVector pos = GetActorLocation();
		if (pos.X > Initpos.X + MovementRange || pos.X < Initpos.X - MovementRange)
		{
			MovementDirecton = !MovementDirecton;
		}
		if (pos.X < Initpos.X + MovementRange && MovementDirecton)
		{
			pos.X += movespeed * DeltaTime;
			SetActorLocation(pos);
		}
		else if (pos.X > Initpos.X - MovementRange && !MovementDirecton)
		{
			pos.X -= movespeed * DeltaTime;
			SetActorLocation(pos);
		}
		
		
	}
}

