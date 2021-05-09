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
}

FVector UGrabber::GetReachEnd() 
{
	FVector playerVPL;
	FRotator playerVPR;

	//Calculate Line Trace End
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(playerVPL, playerVPR);			//This assign the location and rotation of my viewpoint to my 2 parameters
	FVector reach = (playerVPR.Vector()) * reach_length;										//Can get the reach from a player with turning their rotation
	FVector lineTraceEnd = playerVPL + reach;								// can add vectors like this, remember that the linetraceend should be rotation vector place displacment vector form 
	
	return lineTraceEnd;
}

void UGrabber::Grab() 
{
	//grabbing actor that was hit by our reach
	FHitResult Hit = GetObjectHit();

	UPrimitiveComponent* ComponentToGrab = Hit.GetComponent();
	if (Hit.GetActor()) {								//if we hit an actor than grab the actor
		handle->GrabComponentAtLocation(
			ComponentToGrab,
			NAME_None,
			GetReachEnd()								//Reach end calculated by helper function
		);
	}
}

void UGrabber::Released() 
{
	if (handle->GrabbedComponent) {
		handle->ReleaseComponent();						//release the component if we have one
	}
}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	AssignPhysicsHandle();
	AssignInputComponent();
}

void UGrabber::AssignPhysicsHandle()
{
	//Assign our Physics handle component
	handle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();						//match our physics handle component to our member variable
	if (handle)					//ensure our physics handle is not null before working it
	{}
}

void UGrabber::AssignInputComponent()
{
	//Assign our InputComponent
	input = GetOwner()->FindComponentByClass<UInputComponent>();								//match the input component of our actor to our member variable 
	if (input) 
	{
		input->BindAction("Grab", EInputEvent::IE_Pressed, this, &UGrabber::Grab);				//bind our grab code code to our grab key in project settings
		input->BindAction("Grab", EInputEvent::IE_Released, this, &UGrabber::Released);			//bind our release to our grab key so we know when we released
	}
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (handle->GrabbedComponent) {			//keep polling to see if we have a grabbed component
		handle->SetTargetLocation(GetReachEnd());	//set the space where you want the object held
	}
}

FHitResult UGrabber::GetObjectHit()
{
	FVector playerVPL;
	FRotator playerVPR;

	//Calculate Line Trace End
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(playerVPL, playerVPR);			//This assign the location and rotation of my viewpoint to my 2 parameters
	FVector lineTraceEnd = GetReachEnd();

	//Initiate Our line trace to identify what we are hitting
	FHitResult Hit;
	FCollisionQueryParams OmitObjects(FName(TEXT("")), false, GetOwner());
	GetWorld()->LineTraceSingleByObjectType(
		Hit,
		playerVPL,
		lineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),	//Our LineTrace is only going to hit elements in the Physics Body Channel
		OmitObjects															//Omit the actor we have currently attached to this component
	);

	return Hit;
}

