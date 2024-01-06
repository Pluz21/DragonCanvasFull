// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Components/ActorComponent.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DRAGONCANVAS_API UGrabber : public UActorComponent
{

	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

	UPROPERTY(EditAnywhere)
	float grabRadius = 60;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool isGrabbing = false;
	UPROPERTY(EditAnywhere)
	float maxGrabDistance = 600;
	UPROPERTY(EditAnywhere)
	float holdDistance = 200;
	UPROPERTY(EditAnywhere)
	FVector targetLocation = FVector(0);
	UPROPERTY(EditAnywhere)
	TObjectPtr<UPhysicsHandleComponent> physicsHandle;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UPrimitiveComponent> hitComponent;
	UPROPERTY(EditAnywhere)
	FHitResult hitResult;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	bool FindTargetInReach(FHitResult& _hitResult);

	UPhysicsHandleComponent* GetPhysicsHandleComponent();
	UFUNCTION(BlueprintCallable)
	void Grab();
	void Hold();
	void Release();
	UFUNCTION(BlueprintCallable)
	void SetIsGrabbing();
		
};
