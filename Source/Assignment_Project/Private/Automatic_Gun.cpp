// Fill out your copyright notice in the Description page of Project Settings.


#include "Automatic_Gun.h"
#include <DrawDebugHelpers.h>
#include "Assignment_ProjectCharacter.h"

// Sets default values
AAutomatic_Gun::AAutomatic_Gun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("meshComp"));
	RootComponent = RootMesh;
	CamMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Camera_mesh"));
	CamMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AAutomatic_Gun::BeginPlay()
{
	Super::BeginPlay();
	char_ref = UGameplayStatics::GetPlayerCharacter(this, 0);
}

// Called every frame
void AAutomatic_Gun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsActivated)
	{
		Rot = (char_ref->GetMesh()->GetSocketLocation(TEXT("pelvis")) - this->GetActorLocation()).Rotation();
		CamMesh->SetWorldRotation(Rot);
	}
}

void AAutomatic_Gun::Activate()
{
	bIsActivated = true;
	GetWorldTimerManager().SetTimer(FireTimer, this, &AAutomatic_Gun::Fire, Fire_interval, true);
	
}

void AAutomatic_Gun::Fire()
{
	if (char_ref)
	{
		
		FHitResult hitR;
		FCollisionQueryParams Qparams;
		Qparams.AddIgnoredActor(this);
		if(GetWorld()->LineTraceSingleByChannel(hitR, CamMesh->GetSocketLocation("FirePoint"), CamMesh->GetSocketLocation("FirePoint") + Rot.Vector() * 3000, ECC_Visibility, Qparams)) {
			AAssignment_ProjectCharacter* Character_player = Cast<AAssignment_ProjectCharacter>(hitR.Actor);
			if (Character_player)
			{
				Character_player->RecieveDamage(5);
			}
		}
		DrawDebugLine(GetWorld(), CamMesh->GetSocketLocation("FirePoint"), CamMesh->GetSocketLocation("FirePoint") + Rot.Vector() * 3000, FColor::Red, false, 2.f, 0, 5.f);
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), FireParticle, CamMesh->GetSocketLocation("FirePoint"), FRotator::ZeroRotator, true);
	}
}

