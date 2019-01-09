// Copyright Arthur Grohe 2017

#include "Grabber.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "DrawDebugHelpers.h"


#define OUT // does nothing. Just an annotation for us to mark method parameters and visualize that the parameters gets changed

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	Reach = 100.f;
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("Grabber reporting for duty!"));
	
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Get player view point of current tick
	FVector ViewPointLocation;
	FRotator ViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT ViewPointLocation, 
		OUT ViewPointRotation
	);

	//FString loc_Str = ViewPointLocation.ToString();
	//FString rot_Str = ViewPointRotation.ToString();
	//UE_LOG(LogTemp, Warning, TEXT("viewpoint is at %s and looks at %s"), *loc_Str, *rot_Str);

	// draw a trace
	FVector DebugTraceEnd = ViewPointLocation + ViewPointRotation.Vector() * Reach;
	DrawDebugLine(
		GetWorld(),
		ViewPointLocation,
		DebugTraceEnd,
		FColor(255, 0, 0),
		false,
		0,
		0,
		10
	);

	/// Setup query parameters
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());

	FHitResult Hit;
	// do a ray-cast 
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		ViewPointLocation,
		DebugTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams
	);

	AActor *Actor = Hit.GetActor();
	if (Actor) {
		// see what we hit
		UE_LOG(LogTemp, Warning, TEXT("Hitresult: %s"), *(Actor->GetName()) );
	}
}

