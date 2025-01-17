// Copyright © 2023 Pluz21 (TVL). All rights reserved.


#include "DragonCanvas/Actors/Gun.h"
#include "DragonCanvas/Actors/Dragon.h"

// Sets default values
AGun::AGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	gunMesh = CreateDefaultSubobject<UStaticMeshComponent>("gunMesh");
	gunSubMesh = CreateDefaultSubobject<UStaticMeshComponent>("gunSubMesh");
	materialChangerMesh = CreateDefaultSubobject<UStaticMeshComponent>("materialChangerMesh");
	
	gunMesh->SetupAttachment(RootComponent);
	gunSubMesh->SetupAttachment(gunMesh);
	materialChangerMesh->SetupAttachment(gunSubMesh);

}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();
	Init();
	
}

void AGun::Init()
{
	InitPlayerRef();
	InitEvents();
	gunSubMesh->SetVisibility(false,true);
	//GetOnCurrentProjectileMatReceived()
}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Rotate(materialChangerMesh);
}

void AGun::InitPlayerRef()
{
	if (!GetWorld()->GetFirstPlayerController())return;
	ADragon* _playerRef = Cast<ADragon>(GetWorld()->GetFirstPlayerController()->
		GetPawn());
	playerRef = _playerRef;

}

void AGun::InitEvents()
{
	if (!playerRef)return;
	playerRef->GetOnCurrentProjectileMatReceived().AddDynamic(this, &AGun::UpdateBarrelMaterial);
	onBarrelMaterialReceived.AddDynamic(this, &AGun::UpdateBarrelVisibility);
	onBarrelMaterialReceived.AddDynamic(this, &AGun::SetCanRotate);
	
}

void AGun::UpdateBarrelMaterial(UMaterialInterface* _matToApply)
{
	if (!_matToApply)return;
	if (!materialChangerMesh)return;
	materialChangerMesh->SetMaterial(0,_matToApply);
	onBarrelMaterialReceived.Broadcast();
	//Apply mat to all childs if there are any.
	TArray<UStaticMeshComponent*> _allChildStaticMeshes = GetAllChildStaticMeshes(materialChangerMesh);
	int _size = _allChildStaticMeshes.Num();
	if (_size <= 0) return;
	for (int i = 0; i < _size; i++)
		{
			_allChildStaticMeshes[i]->SetMaterial(0, _matToApply);
		}

}

TArray<UStaticMeshComponent*> AGun::GetAllChildStaticMeshes(UStaticMeshComponent* _parentMesh)
{
	TArray <USceneComponent*> _allSceneComps;
	TArray<UStaticMeshComponent*> _allChildStaticMeshes;
	_allSceneComps = _parentMesh->GetAttachChildren();

	if (_allSceneComps.Num() <= 0) return _allChildStaticMeshes;

	for (USceneComponent* _childSceneCompo : _allSceneComps)
	{
		UStaticMeshComponent* _childStaticMesh = Cast<UStaticMeshComponent>(_childSceneCompo);
		
		_allChildStaticMeshes.Add(_childStaticMesh);
	}
	return _allChildStaticMeshes;
	
}



void AGun::UpdateBarrelVisibility()
{
	if (!playerRef)return;
	if (!gunSubMesh->GetVisibleFlag())
	{
		gunSubMesh->SetVisibility(true,true);

	}
	
}

void AGun::SetCanRotate()
{
	canRotate = true;
}

void AGun::Rotate(UStaticMeshComponent* _meshToRotate)
{
	if (!canRotate)return;
	float _rotateSpeed = rotateSpeed * GetWorld()->DeltaTimeSeconds;


	FRotator _currentRotation = _meshToRotate->GetComponentRotation();
	FRotator _newRotation = FRotator(0, _rotateSpeed, 0);

	_meshToRotate->SetWorldRotation(_currentRotation + _newRotation);
}

void AGun::GunSwap(AGun* _gunToUse)
{
	APawn* _pawn = GetWorld()->GetFirstPlayerController()->GetPawn();
	if (!_pawn)return;

	ADragon* _player = Cast<ADragon>(_pawn);
	_player->AttachGun(_gunToUse, _player->gunSpawnPoint);

}

