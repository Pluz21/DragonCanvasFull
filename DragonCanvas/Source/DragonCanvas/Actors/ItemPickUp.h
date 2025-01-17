// Copyright © 2023 Pluz21 (TVL). All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemPickUp.generated.h"

class UCapsuleComponent;
class ADragon;

UCLASS(Abstract)
class DRAGONCANVAS_API AItemPickUp : public AActor
{
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOverlapFinishedEvent);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInitEvent);
	GENERATED_BODY()
	
protected:
	UPROPERTY()
	FOverlapFinishedEvent onOverlapFinished;
	UPROPERTY()
	FInitEvent onInit;
	
public:	
	// Sets default values for this actor's properties
	AItemPickUp();

	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> meshCompo;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UCapsuleComponent> capsuleCollider;
	UPROPERTY(EditAnywhere)
	TObjectPtr<ADragon> playerRef;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> pickUpSound = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool canRotate = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float rotateSpeed = 60;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float randFloat = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool canMove = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float moveSpeed = 30;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float maxDistanceToTravel = 50;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector currentLocation = FVector(0);
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector targetLocation = FVector(0);
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector initialLocation = FVector(0);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Init();
	virtual void GetPlayerRef();
	UFUNCTION()
	virtual void ManageOverlap(AActor* _overlapped, AActor* _overlap);
	UFUNCTION()
	virtual void ItemAction();
	virtual void Rotate();

	virtual void Move();

	virtual void DistanceCheck();

	virtual void UpdateCurrentLocation() { currentLocation = GetActorLocation(); }
	virtual void GetInitialLocation() { initialLocation = GetActorLocation(); }
	
	virtual FOverlapFinishedEvent& GetOnOverlapFinished() { return onOverlapFinished; };

};
