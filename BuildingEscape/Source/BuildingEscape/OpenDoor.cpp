#include "OpenDoor.h"
#include "BuildingEscape.h"
#include "Engine/World.h"

#define OUT

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	Owner = GetOwner();
	if(!PressurePlate){
		UE_LOG(LogTemp, Error, TEXT("%s missing pressure plate"), *GetOwner()->GetName());
	}

}




// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


	// Poll the Trigger Volume
	if (GetTotalMassOfActorsOnPlate()>TriggerMass) { 
		OnOpen.Broadcast();
	}
	else{
		OnClose.Broadcast();
	}
		
}


float UOpenDoor::GetTotalMassOfActorsOnPlate() {

	float TotalMass = 0.f;

	// Find all the overlapping actors
	TArray<AActor*> OverlappingActors;
	if (!PressurePlate) { return TotalMass; } 
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);

	// Iterate through them adding their mass
	for(const auto* Actor : OverlappingActors){
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		UE_LOG(LogTemp, Warning, TEXT("%s on pressure plate"), *Actor->GetName());
	}

	return TotalMass;
}
