// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/TriggerVolume.h"
#include "Components/ActorComponent.h"
#include "OpenDoor.generated.h"



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();
	
	//Opens the Door
	void OpenDoor();

	void CloseDoor();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

		
private:
		UPROPERTY(EditAnywhere)
		float OpenAngle = 60.f;

		UPROPERTY(EditAnywhere)
		ATriggerVolume* Pressureplate;
		
		UPROPERTY(EditAnywhere)
		AActor* ActorThatOpens;

		UPROPERTY(EditAnywhere)
		float DoorCloseDelay=1.0f;
		
		float LastDoorOpenTime;
		AActor* Owner;
};
