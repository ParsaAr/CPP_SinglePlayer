// Fill out your copyright notice in the Description page of Project Settings.


#include "AlarmLights.h"

// Sets default values
AAlarmLights::AAlarmLights()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("meshcomp"));
	RootComponent = meshComp;
	lightcomp = CreateDefaultSubobject<UPointLightComponent>(TEXT("LightComp"));
	lightcomp->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AAlarmLights::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAlarmLights::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAlarmLights::ActivateAlarm()
{
	lightcomp->SetLightColor(ActivationColor);
}

