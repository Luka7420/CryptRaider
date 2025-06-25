// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber.h"

#include "Engine/World.h"

#include "DrawDebugHelpers.h"

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
	// Call the parent class's TickComponent to ensure any inherited behavior runs
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	
}
void UGrabber::Release()
{
	UE_LOG(LogTemp, Display, TEXT("Released grabber"));
}
void UGrabber::Grab()
{
	
	// Get the current location of this component (the start point for the debug line)
	FVector Start = GetComponentLocation();
	// Calculate the end point by moving forward from the start by MaxGrabDistance units
	FVector End = Start + GetForwardVector() * MaxGrabDistance;
	// Draw a red debug line in the world from Start to End to visualize the grab range
	DrawDebugLine(GetWorld(), Start, End, FColor::Red);
	
	FCollisionShape Sphere = FCollisionShape::MakeSphere(GrabRadius);
	FHitResult HitResult;
	bool HasHit = GetWorld()->SweepSingleByChannel(
		HitResult, 
		Start, End, 
		FQuat::Identity, 
		ECC_GameTraceChannel2, 
		Sphere
	);
	if(HasHit)
	{
		AActor* HitActor = HitResult.GetActor();
		UE_LOG(LogTemp, Display, TEXT("Hit actor: %s"), *HitActor->GetActorNameOrLabel());
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("No actor hit within grab range."));
	}
}