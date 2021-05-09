// Fill out your copyright notice in the Description page of Project Settings.


#include "OpenDoor.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Actor.h"
#include "Components/PrimitiveComponent.h"


// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	//Assign Our member variable to our component on the actor
	DoorSound = GetOwner()->FindComponentByClass<UAudioComponent>();

	//Assign Our relative measurement to open door in our world
	initialYaw = GetOwner()->GetActorRotation().Yaw;
	currentYaw = initialYaw;
	targetYaw += initialYaw;									//this way it doesnt matter if door frame starts on 0 or 180 degrees it will still close.
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//Asses whether to open or close the doors
	if (PressurePlate && TotalMassOfActors() > MassToOpenDoors) //check for if our trigger volume exists and how much weight is on it
	{
		OpenDoor(DeltaTime);
		lastOpened = GetWorld()->GetTimeSeconds();
		DoorSoundPlayed = true;
	}
	else {
		if ((GetWorld()->GetTimeSeconds() - lastOpened) > CloseSpeed) {		//when the door has been opened at least have it open for about 2 seconds before closing it.
			CloseDoor(DeltaTime);
			DoorSoundPlayed = false;
		}
	}
}

float UOpenDoor::TotalMassOfActors() const
{
	float TotalMass = 0.f;
	TArray <AActor*> ActorsInTrigger;
	PressurePlate->GetOverlappingActors(ActorsInTrigger);

	//Get the total mass of actors in trigger
	for (AActor* actor : ActorsInTrigger)
	{
		TotalMass += actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();		//get the mass of the actors primitive component(the static mesh/collision comp)
	}
	return TotalMass;
}
void UOpenDoor::OpenDoor(float DeltaTime) 
{
	if (!DoorSoundPlayed) { DoorSound->Play(); }								//use bool to check if door sound has been played once for every open and close. On open set bool true. on close set bool false
	if (currentYaw == initialYaw) { DoorSound->Play(); }						//only play sound once and thats when the door begins open
	FRotator Opendoor = GetOwner()->GetActorRotation();
	currentYaw = FMath::FInterpTo(currentYaw, targetYaw, DeltaTime, 0.5f);		//continually increase the current yaw to our target yaw
	Opendoor.Yaw = currentYaw;													
	GetOwner()->SetActorRotation(Opendoor);										//set out current yaw to our continually updated value

}
void UOpenDoor::CloseDoor(float DeltaTime) 
{
	if (DoorSoundPlayed) { DoorSound->Play(); }								//use bool to check if door sound has been played once for every open and close. On open set bool true. on close set bool false
	if (currentYaw == initialYaw) { DoorSound->Play(); }
	FRotator Closedoor = GetOwner()->GetActorRotation();
	currentYaw = FMath::FInterpTo(currentYaw, initialYaw, DeltaTime, 2.f);		//continually increase the current yaw to our target yaw
	Closedoor.Yaw = currentYaw;
	GetOwner()->SetActorRotation(Closedoor);										//set out current yaw to our continually updated value
}

