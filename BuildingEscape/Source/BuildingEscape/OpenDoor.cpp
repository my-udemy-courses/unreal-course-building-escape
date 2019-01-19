// Copyright Arthur Grohe 2017

#include "OpenDoor.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Components/PrimitiveComponent.h"

#define OUT

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	// Find the owning Actor
	Owner = this->GetOwner();
	if (!PressurePlate) {
		UE_LOG(LogTemp, Error, TEXT("Unable to find PressurePlate component in %s"), *GetOwner()->GetName())
	}
}

void UOpenDoor::OpenDoor()
{
	Owner->SetActorRotation(FRotator(0.f, OpenAngle, 0.f), ETeleportType::TeleportPhysics);
}

void UOpenDoor::CloseDoor()
{
	Owner->SetActorRotation(FRotator(0.f, 0.f, 0.f), ETeleportType::TeleportPhysics);
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	float CurrentTime = GetWorld()->GetTimeSeconds();

	// Poll the trigger volume
	if (GetTotalMassOfActorsOnPlate() > 50.0f) { // TODO: kill magic number
		OpenDoor();
		LastTimeDoorOpened = CurrentTime;
	}

	if (CurrentTime - LastTimeDoorOpened > CloseDoorDelay) {
		CloseDoor();
	}
	
}

float UOpenDoor::GetTotalMassOfActorsOnPlate()
{
	float TotalMass = 0.f;

	// Find all the overlapping actors
	TArray<AActor *> OverlappingActors;
	if (!PressurePlate) return -1.f;
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);

	// Iterate through them adding their masses
	for (const auto* Actor : OverlappingActors)
	{
		UE_LOG(LogTemp, Warning, TEXT("Overlapping: %s"), *Actor->GetName());
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}

	return TotalMass;
}
