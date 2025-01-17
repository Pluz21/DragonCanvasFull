// Copyright © 2023 Pluz21 (TVL). All rights reserved.


#include "DragonCanvas/Actors/ItemPickUp.h"
#include "DragonCanvas/Actors//Dragon.h"

#include "Components/CapsuleComponent.h"

#include "Kismet/GameplayStatics.h"




// Sets default values
AItemPickUp::AItemPickUp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	meshCompo = CreateDefaultSubobject<UStaticMeshComponent>("meshCompo");
	capsuleCollider = CreateDefaultSubobject<UCapsuleComponent>("capsuleCollider");
	meshCompo->SetupAttachment(RootComponent);
	capsuleCollider->SetupAttachment(meshCompo);

}

// Called when the game starts or when spawned
void AItemPickUp::BeginPlay()
{
	Super::BeginPlay();
	Init();
	
}

// Called every frame
void AItemPickUp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Rotate();
	UpdateCurrentLocation();
	Move();
}

void AItemPickUp::Init()
{
	OnActorBeginOverlap.AddDynamic(this, &AItemPickUp::ManageOverlap);
	onOverlapFinished.AddDynamic(this, &AItemPickUp::ItemAction);
	GetPlayerRef();
	randFloat = FMath::RandRange(0.5f, 2.f);
	GetInitialLocation();
	currentLocation = initialLocation;
	targetLocation = initialLocation + FVector(0, 0, 50);
}

void AItemPickUp::GetPlayerRef()
{
	APawn* _pawn = GetWorld()->GetFirstPlayerController()->GetPawn();
	if (!_pawn)return;

	ADragon* _player = Cast<ADragon>(_pawn);
	if (!_player)return;
	playerRef = _player;
}

void AItemPickUp::ManageOverlap(AActor* _overlapped, AActor* _overlap)
{
	UE_LOG(LogTemp, Warning, TEXT("Overlapping with %s"), *_overlap->GetName());
	
	if (!_overlapped || !_overlap)return;
	if (!_overlap->IsA<ADragon>())return;
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), pickUpSound,GetActorLocation());
	onOverlapFinished.Broadcast(); // Calling after destroy is causing issues
	_overlapped->Destroy();
}

void AItemPickUp::Rotate()
{
	if (!canRotate) return;


	float _rotateValue = rotateSpeed * GetWorld()->DeltaTimeSeconds * randFloat;
	FRotator _currentRotation = GetActorRotation();
	FRotator _newRotation = FRotator(0, _rotateValue, 0);

	if (randFloat < 1.2f)
	{
		_newRotation *= -1;
	}

	SetActorRotation(_currentRotation + _newRotation);
}

void AItemPickUp::Move()
{
	DistanceCheck();
	float _speed = moveSpeed * randFloat * GetWorld()->DeltaTimeSeconds;
	FVector _moveDir = (targetLocation - currentLocation).GetSafeNormal();
	FVector _newLocation = GetActorLocation() + _moveDir * _speed;
	SetActorLocation(_newLocation);

}

void AItemPickUp::DistanceCheck()
{
	float _distance = FVector::Dist(initialLocation, currentLocation);
	if (_distance >= maxDistanceToTravel)
	{
		FVector _newTargetLocation = initialLocation;
		initialLocation = currentLocation;
		targetLocation = _newTargetLocation;
	}

	//if (_distance <= maxDistanceToTravel)
		//UE_LOG(LogTemp, Warning, TEXT("NOT REACHED"));
}

void AItemPickUp::ItemAction()
{

}





