// Fill out your copyright notice in the Description page of Project Settings.


#include "SafeArea.h"

// Sets default values
ASafeArea::ASafeArea()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	boxcol = CreateDefaultSubobject<UBoxComponent>(TEXT("boxcol"));

	boxcol->OnComponentBeginOverlap.AddDynamic(this, &ASafeArea::EnterSafeArea);
}

// Called when the game starts or when spawned
void ASafeArea::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASafeArea::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASafeArea::EnterSafeArea(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AAssignment_ProjectCharacter* charref = Cast<AAssignment_ProjectCharacter>(OtherActor);
	if (charref)
	{
		if (charref->TimerStatus)
		{
			charref->IsInSafeArea = true;
			charref->WinTheGame();
		}
	}
}

