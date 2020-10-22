// Fill out your copyright notice in the Description page of Project Settings.


#include "DeadZone.h"
#include "Assignment_ProjectCharacter.h"

// Sets default values
ADeadZone::ADeadZone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	boxcomp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	RootComponent = boxcomp;
	boxcomp->OnComponentBeginOverlap.AddDynamic(this, &ADeadZone::BoxBegingOverlapp);
}

// Called when the game starts or when spawned
void ADeadZone::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADeadZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADeadZone::BoxBegingOverlapp ( UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AAssignment_ProjectCharacter* charref = Cast<AAssignment_ProjectCharacter>(OtherActor);
	if (charref)
	{
		charref->LostTheGame();
	}
}

