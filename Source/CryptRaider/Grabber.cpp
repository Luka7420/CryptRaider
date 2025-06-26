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
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	// Call the parent class's TickComponent to ensure any inherited behavior runs
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();
	if(PhysicsHandle == nullptr)
	{
		return;
	}
	
	if(PhysicsHandle->GetGrabbedComponent() != nullptr)
	{
		FVector TargetLocation = GetComponentLocation() + GetForwardVector() * HoldDistance; // Calculate the target location for the grabbed component, which is the current location of this component plus an offset in the forward direction
		PhysicsHandle->SetTargetLocationAndRotation(TargetLocation, GetComponentRotation()); // Set the target location and rotation of the physics handle to the calculated target location and the current rotation of this component
	}
	
	
}

void UGrabber::Grab()
{
	UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();
	if(PhysicsHandle == nullptr)
	{
		return;
	}
	
	FHitResult HitResult;
	bool HasHit = GetGrabbableInReach(HitResult); // Check if there is a grabbable object in reach and store the result in HitResult
	if(HasHit)
	{
		// Grabbing code bellow
		UPrimitiveComponent* HitComponent = HitResult.GetComponent();
		HitComponent->WakeAllRigidBodies();
		PhysicsHandle->GrabComponentAtLocationWithRotation(
			HitComponent,
			NAME_None,
			HitResult.ImpactPoint,
			GetComponentRotation()
		);
	}
	
}
void UGrabber::Release()
{
	UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();
	if(PhysicsHandle == nullptr)
	{
		return;
	}
	if(PhysicsHandle->GetGrabbedComponent() != nullptr)
	{
		PhysicsHandle->GetGrabbedComponent()->WakeAllRigidBodies();// Wake the grabbed component to ensure it responds to physics immediately
		PhysicsHandle->ReleaseComponent(); // Release the grabbed component
	}
}
UPhysicsHandleComponent* UGrabber::GetPhysicsHandle() const
{
	UPhysicsHandleComponent* Result = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>(); // Find the PhysicsHandleComponent on the owner actor	
	if(Result == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Grabber component requires a PhysicsHandleComponent on the owner actor!"));
	}
	return Result;
}
bool UGrabber::GetGrabbableInReach(FHitResult& OutHitResult) const
{
	// Get the current location of this component (the start point for the debug line)
	FVector Start = GetComponentLocation();
	// Calculate the end point by moving forward from the start by MaxGrabDistance units
	FVector End = Start + GetForwardVector() * MaxGrabDistance;
	// Draw a red debug line in the world from Start to End to visualize the grab range
	DrawDebugLine(GetWorld(), Start, End, FColor::Red);
	DrawDebugSphere(GetWorld(), End, 10, 10, FColor::Blue, false, 5);
	
	FCollisionShape Sphere = FCollisionShape::MakeSphere(GrabRadius);
	return GetWorld()->SweepSingleByChannel(
		OutHitResult, 
		Start, End, 
		FQuat::Identity, 
		ECC_GameTraceChannel2, 
		Sphere);
}