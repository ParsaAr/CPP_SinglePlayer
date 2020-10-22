// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Gameframework/Character.h"
#include <Particles/ParticleSystem.h>
#include "Automatic_Gun.generated.h"

UCLASS()
class AAutomatic_Gun : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAutomatic_Gun();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* RootMesh;

	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* CamMesh;

	UPROPERTY(EditAnywhere)
		UParticleSystem* FireParticle;
	bool bIsActivated;

	void Activate();
	UFUNCTION()
		void Fire();
	FTimerHandle FireTimer;

	UPROPERTY(EditAnywhere)
		float Fire_interval;

	ACharacter* char_ref;

	FRotator Rot;
	
};
