//Copyright © 2023 Pluz21(TVL).All rights reserved.

#include "PickUps.h"

// Sets default values
APickUps::APickUps()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	meshCompo = CreateDefaultSubobject<UStaticMeshComponent>("meshCompo");
	meshCompo->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void APickUps::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APickUps::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Rotate();
}

void APickUps::Rotate()
{
	
	if (!canRotate) return;

	float _rotateValue = rotateSpeed * GetWorld()->DeltaTimeSeconds;
	FRotator _currentRotation = GetActorRotation();
	FRotator _newRotation = FRotator(0, _rotateValue, 0);

	SetActorRotation(_currentRotation + _newRotation);
	
}

