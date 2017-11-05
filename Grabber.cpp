// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildingEscape.h"
#include "Grabber.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	SetPhysicsComponent();
	SetInputComponent();	
}

void UGrabber::SetPhysicsComponent()
{
	//Look for attached Physics Handle
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle==nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Physics Handle Not Found for %s"), *GetOwner()->GetName());
	}
}

void UGrabber::SetInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		//Grab/Release Functions get Binded to Keys "IE_Presses/Released"
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Input Component Not Found for %s"), *GetOwner()->GetName());
	}
}

// Called every frame
void UGrabber::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );
	
	//Moves object that we're holding
	if (PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->SetTargetLocation(GetReachLineEnd());
	}
	
	//Debugging stuff in case we want to use it for new code below

	//UE_LOG(LogTemp, Warning, TEXT("Location: %s Rotation: %s!"),
	//	*PlayerViewPointLocation.ToString(),
	//	*PlayerViewPointRotation.ToString());
	/* Debugging Line
	DrawDebugLine(GetWorld(),
		PlayerViewPointLocation,
		LineTraceEnd,
		FColor(255, 0, 0),
		false,
		0.f,
		0.f,
		10.f);
	*/
}

void UGrabber::Grab()
{
	//**LINE TRACE and see if we reach any Actors with collision type Physics Body**
	auto HitResult = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent();
	auto ActorHit = HitResult.GetActor();

	if (ActorHit)
	{	
		//Attaches physics handle
		/*Other Ways of attaching
		PhysicsHandle->GrabComponent(
			ComponentToGrab,
			NAME_None,
			ComponentToGrab->GetOwner()->GetActorLocation(),
			true);
		
			PhysicsHandle->GrabComponentAtLocation(ComponentToGrab,
			NAME_None,
			ComponentToGrab->GetOwner()->GetActorLocation());
		*/
		PhysicsHandle->GrabComponentAtLocationWithRotation(
			ComponentToGrab,
			NAME_None,
			ComponentToGrab->GetOwner()->GetActorLocation(),
			ComponentToGrab->GetOwner()->GetActorRotation());
	}
	return;
}

void UGrabber::Release()
{
	PhysicsHandle->ReleaseComponent();
	return;
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	//What we hit with Ray-Cast
	FHitResult LineTraceHit;

	//Set Collision Parameters
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());

	//Line-trace (AKA Ray-cast) out to reach distance
	GetWorld()->LineTraceSingleByObjectType(
		OUT LineTraceHit,
		GetReachLineStart(),
		GetReachLineEnd(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams);

	/*
	AActor* ActorHit;
	//See what we hit
	ActorHit = LineTraceHit.GetActor();
	if (ActorHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("Line Trace Hit: %s!"), *ActorHit->GetName());
	}
	*/
	return LineTraceHit;
}

FVector UGrabber::GetReachLineStart()
{
	//Regarding reach vectors
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	//Update Ray-Cast Vectors
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation);
	return PlayerViewPointLocation;
}

FVector UGrabber::GetReachLineEnd()
{
	//Regarding reach vectors
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	//Update Ray-Cast Vectors
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation);
	return FVector(PlayerViewPointLocation + PlayerViewPointRotation.Vector()*Reach);
}
