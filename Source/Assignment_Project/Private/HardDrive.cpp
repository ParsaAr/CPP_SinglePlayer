// Fill out your copyright notice in the Description page of Project Settings.


#include "HardDrive.h"
#include "Assignment_ProjectCharacter.h"
// Sets default values
AHardDrive::AHardDrive()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	meshcomp= CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = meshcomp;
	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &AHardDrive::OverLapEventStart);
	SphereCollision->OnComponentEndOverlap.AddDynamic(this, &AHardDrive::EndOverLapEvent);
}

// Called when the game starts or when spawned
void AHardDrive::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHardDrive::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHardDrive::EndOverLapEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AAssignment_ProjectCharacter* charref = Cast<AAssignment_ProjectCharacter>(OtherActor);
	if (charref)
	{
		charref->UiMessage(false);
		charref->Availableitem = false;
		charref->canPickupItem = false;
	}
}

void AHardDrive::OverLapEventStart(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AAssignment_ProjectCharacter* charref = Cast<AAssignment_ProjectCharacter>(OtherActor);
	if (charref)
	{
		
		charref->UiMessage(true);
		charref->canPickupItem = true;
		charref->Availableitem = this;
	}
}

