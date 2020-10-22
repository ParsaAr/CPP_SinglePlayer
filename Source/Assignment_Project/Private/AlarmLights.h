// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/PointLightComponent.h"
#include <Components/StaticMeshComponent.h>
#include "AlarmLights.generated.h"

UCLASS()
class AAlarmLights : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAlarmLights();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
		UPointLightComponent* lightcomp;
	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* meshComp;
	UPROPERTY(EditAnywhere)
		FLinearColor ActivationColor;
	void ActivateAlarm();
};	
