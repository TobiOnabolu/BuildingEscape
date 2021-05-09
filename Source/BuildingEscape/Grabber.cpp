// Fill out your copyright notice in the Description page of Project Settings.
#include "DrawDebugHelpers.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "Math/Color.h"
#include "Grabber.h"

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	FVector playerVPL;
	FRotator playerVPR;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(playerVPL, playerVPR);	//This assign the location and rotation of my viewpoint to my 2 parameters

	FVector reach = (playerVPR.Vector()) * reach_length;			//Can get the reach from a player with turning their rotation

	FVector lineTraceEnd = playerVPL + reach;				// can add vectors like this, remember that the linetraceend should be rotation vector place displacment vector form origin

	DrawDebugLine(
		GetWorld(),
		playerVPL,
		lineTraceEnd,
		FColor(0, 255, 0),
		false,
		0.f,
		0,
		5);
	/*UE_LOG(LogTemp, Warning, TEXT("Location: %s \nRoatation: %s"), *playerVPL.ToString(), *playerVPR.ToString());*/

	FHitResult Hit;
	FCollisionQueryParams OmitObjects(FName(TEXT("")), false, GetOwner());

	GetWorld()->LineTraceSingleByObjectType(
		Hit,
		playerVPL,
		lineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		OmitObjects
	);

	AActor* ActorHit = Hit.GetActor();
	if (ActorHit) {
		UE_LOG(LogTemp, Warning, TEXT("%s"), *ActorHit->GetName());
	}
}

