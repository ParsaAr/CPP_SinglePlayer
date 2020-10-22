// Fill out your copyright notice in the Description page of Project Settings.


#include "Robot.h"
#include <Engine/Engine.h>
#include <DrawDebugHelpers.h>
#include <Kismet/GameplayStatics.h>
#include "Assignment_ProjectCharacter.h"
#include <UnrealMathUtility.h>

// Sets default values
ARobot::ARobot()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	meshcomp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = meshcomp;
	spherecomp = CreateDefaultSubobject<USphereComponent>(TEXT("Spherecomponent"));
	spherecomp->SetupAttachment(RootComponent);
	Attacking = false;
	MoveSpeed = 100;
	DistancetoStartBinking = 500;
	DistancetoExplode = 100;
	RobotHealth = 100;
}

// Called when the game starts or when spawned
void ARobot::BeginPlay()
{
	Super::BeginPlay();
	D_Mat = meshcomp->CreateDynamicMaterialInstance(0);
	Char_ref = UGameplayStatics::GetPlayerCharacter(this, 0);
}


void ARobot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (Attacking)
	{	
		
		
		if (Char_ref)
		{
			
			FVector DirectionToGo = Char_ref->GetActorLocation() - this->GetActorLocation();
			float Distance = FMath::Max(0.f, DirectionToGo.Size());
			
			DirectionToGo.Normalize();
			this->SetActorLocation(GetActorLocation() + DirectionToGo * 3);
			if (D_Mat)
			{
				if (Distance > DistancetoStartBinking)
				{
					D_Mat->SetVectorParameterValue("BlinkingColor", FLinearColor::Yellow);
					D_Mat->SetScalarParameterValue("BlinkingSpeed", 2.f);
					D_Mat->SetScalarParameterValue("BlinkingPower", 2.f);
				}

				if (Distance < DistancetoStartBinking)
				{
					D_Mat->SetScalarParameterValue("BlinkingSpeed", 4.f);
					D_Mat->SetScalarParameterValue("BlinkingPower", 5.f);
					D_Mat->SetVectorParameterValue("BlinkingColor", FLinearColor::Red);

					if (Distance < DistancetoExplode)
					{
						Explosion();
					}

				}
			}
		}
	}
}

void ARobot::Explosion()
{ // add explosion with timer
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),ExplosionParticle, this->GetTransform() );
	if (spherecomp->IsOverlappingActor(Char_ref))
	{
		AAssignment_ProjectCharacter* CharacterREF = Cast<AAssignment_ProjectCharacter>(Char_ref);
		if (CharacterREF)
		{
			CharacterREF->RecieveDamage(DamageToapply);
		}
	}
	this->Destroy();
}

void ARobot::RecieveDamage(float Dmg_amount)
{
	RobotHealth = FMath::Clamp(RobotHealth - Dmg_amount, 0.0f, 100.0f);
	if (RobotHealth <= 0.0f) {
		Explosion();
		return;
	}
	
}

