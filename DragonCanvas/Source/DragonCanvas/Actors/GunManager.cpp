// Copyright © 2023 Pluz21 (TVL). All rights reserved.


#include "DragonCanvas/Actors/GunManager.h"

// Sets default values
AGunManager::AGunManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGunManager::BeginPlay()
{
	Super::BeginPlay();
	Init();
}

// Called every frame
void AGunManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGunManager::Init()
{
	APawn* _playerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
	playerRef = Cast<ADragon>(_playerPawn);
	

}

TObjectPtr<AGun> AGunManager::GetGun()
{
	int _size = allGuns.Num();
	for (size_t i = 0; i < _size; i++)
	{
		if (allGuns[i]->IsA<ALaserGun>())
			return allGuns[i];
		else if (allGuns[i]->IsA<ABaseGun>())
			return allGuns[i];
	}
	UE_LOG(LogTemp, Warning, TEXT("ROOT FUNCTION NOT FOUND"));

	return nullptr;
	/*if (_size >= 0 && _index >= 0 && _index < _size)
		return allGuns[_index];
	return nullptr;*/
}
void AGunManager::AddItem(TObjectPtr<AGun> _item)
{
	if (!_item || Exists(_item))return; // check if item is !valid and if he has already been added to array
	allGuns.Add(_item);
}

void AGunManager::RemoveItem(TObjectPtr<AGun> _item)
{
	if (!Exists(_item))return; //check if item exists in array.
	allGuns.Remove(_item);
}

bool AGunManager::Exists(TObjectPtr<AGun> _item)
{
	int _size = allGuns.Num();
	for (int i = 0; i < _size; i++)
	{
		AGun* _gun = allGuns[i];
		if (!_gun)continue;
		if (_gun == _item)
			return true;
	}
	return false;
}

bool AGunManager::Exists(const int& _index)
{
	int _size = allGuns.Num();
	if (_index < 0 || _index >= _size)return false; // check out of the bounds of array
	AGun* _gun = allGuns[_index];
	if (!_gun)return false;
	return true;
}


