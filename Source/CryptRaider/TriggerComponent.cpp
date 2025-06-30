// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerComponent.h"

UTriggerComponent::UTriggerComponent()
{
    // Set this component to be initialized when the game starts, and to be ticked every frame.
    PrimaryComponentTick.bCanEverTick = true;

    
}

void UTriggerComponent::BeginPlay()
{
	Super::BeginPlay();
    
}
void UTriggerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) 
{
    // Call the parent class's TickComponent to ensure any inherited behavior runs
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    TArray<AActor*> Actors;
    GetOverlappingActors(Actors);

    
    for(AActor* Actor : Actors)
    {
      if(Actor->ActorHasTag(AcceptableActorTag))
      {
        UE_LOG(LogTemp, Display, TEXT("Unlocking"));
      }
    }
    
}