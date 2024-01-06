//Copyright © 2023 Pluz21(TVL).All rights reserved.

#include "Spawner.h"
#include "Components/BillboardComponent.h"


// Sets default values
ASpawner::ASpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	root = CreateDefaultSubobject<USceneComponent>("root");
	billboardComponent= CreateDefaultSubobject<UBillboardComponent>("billboard");
	billboardComponent->SetupAttachment(root);
}

// Called when the game starts or when spawned
void ASpawner::BeginPlay()
{
	Super::BeginPlay();
	Init();
}

// Called every frame
void ASpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASpawner::Init()
{
	currentLocation = GetActorLocation();
}

AActor* ASpawner::Spawn()
{
	AActor* _spawned = GetWorld()->SpawnActor<AActor>(actorToSpawn, 
		GetActorLocation(), GetActorRotation());
	UE_LOG(LogTemp, Error, TEXT("SPAWNED"));
	allSpawned.Add(_spawned);
	if (!_spawned)return nullptr;
	return _spawned;


}

void ASpawner::Remove()
{
	if (allSpawned.Num() < 1)return;
	AActor* _lastSpawned = allSpawned.Last();
	if (!_lastSpawned)return;
	_lastSpawned->Destroy();
	allSpawned.Remove(_lastSpawned);
}


void ASpawner::RemoveAll()
{
	if (allSpawned.Num() < 1)return;
	int _size = allSpawned.Num();
	for (size_t i = 0; i < _size ; i++)
	{
		AActor* _spawned = allSpawned[i];
		if (!_spawned) continue;
		_spawned->Destroy();
	}
	allSpawned.Empty();

}

