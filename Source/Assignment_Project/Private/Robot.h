// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "components/SphereComponent.h"
#include "components/StaticMeshComponent.h"
#include "Assignment_ProjectCharacter.h"
#include "Particles/ParticleSystem.h"
#include "GameFramework/Character.h"
#include "Robot.generated.h"

UCLASS()
class ARobot : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARobot();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(VisibleAnywhere)
		USphereComponent* spherecomp;
	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* meshcomp;
	bool Attacking;
	UPROPERTY(EditAnywhere , BlueprintReadOnly)
		float MoveSpeed;

	UMaterialInstanceDynamic* D_Mat;
	
	UPROPERTY(EditAnywhere)
		float DistancetoStartBinking;
	UPROPERTY(EditAnywhere)
		float DistancetoExplode;
	UPROPERTY(EditAnywhere)
		UParticleSystem* ExplosionParticle;
	void Explosion();

		ACharacter* Char_ref;
	
		UPROPERTY(EditAnywhere)
			float DamageToapply;
		
		float RobotHealth;

		void RecieveDamage(float Dmg_amount);

		
};
