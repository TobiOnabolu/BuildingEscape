// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "GameFramework/Actor.h"
#include "Components/AudioComponent.h"
#include "OpenDoor.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void OpenDoor(float DeltaTime);
	void CloseDoor(float DeltaTime);
	float TotalMassOfActors() const;

private:
	float initialYaw;
	float currentYaw;
	float lastOpened = 0.f;
	bool DoorSoundPlayed = false;

	UPROPERTY(EditAnywhere)
		float MassToOpenDoors = 50.f;

	UPROPERTY(EditAnywhere)
		float targetYaw;

	UPROPERTY(EditAnywhere)
		float CloseSpeed = 0.05f;

	UPROPERTY(EditAnywhere)
		ATriggerVolume* PressurePlate = nullptr;
	
	UPROPERTY()
	UAudioComponent* DoorSound = nullptr;
};
