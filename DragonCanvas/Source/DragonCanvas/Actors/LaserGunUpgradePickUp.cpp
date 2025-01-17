// Copyright © 2023 Pluz21 (TVL). All rights reserved.


#include "DragonCanvas/Actors/LaserGunUpgradePickUp.h"
#include "Dragon.h"
#include "Gun.h"
#include "GunManager.h"
#include "DragonCanvas/Actors/PlayerProjectile.h"
#include "DragonCanvas/Components/ManaComponent.h"


ALaserGunUpgradePickUp::ALaserGunUpgradePickUp()
{
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ALaserGunUpgradePickUp::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ALaserGunUpgradePickUp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALaserGunUpgradePickUp::TriggerPlayerGunSwap()
{

	UE_LOG(LogTemp, Warning, TEXT("GunSwap triggered"));
	APawn* _pawn = GetWorld()->GetFirstPlayerController()->GetPawn();
	if (!_pawn)return;

	ADragon* _player = Cast<ADragon>(_pawn);
	_player->AttachGun(_player->laserGunRef, _player->gunSpawnPoint);

	_player->gunManager->AddItem(_player->laserGunRef);
	AGun* _currentGun = _player->GetCurrentGun();
	if (!_currentGun)
	{
		UE_LOG(LogTemp, Warning, TEXT("No current gun found"));

	}
	UE_LOG(LogTemp, Warning, TEXT("Current gun is : %s"), *_currentGun->GetPathName());
}


void ALaserGunUpgradePickUp::UpdatePlayerProjectileMesh()
{
	
	APlayerProjectile* _projectileRef = playerRef->projectileToSpawn.GetDefaultObject();
	if (!_projectileRef)return;
	UStaticMeshComponent* _projectileMeshComponent = _projectileRef->
		GetComponentByClass<UStaticMeshComponent>();
	if (!_projectileMeshComponent || !laserMesh)return;
	_projectileMeshComponent->SetStaticMesh(laserMesh);
	onMeshUpdated.Broadcast(_projectileMeshComponent);
}

void ALaserGunUpgradePickUp::UpdateProjectileManaCost()
{
	
	playerRef->manaCompo->SetProjectileManaCost(laserProjectileManaCost);
}

void ALaserGunUpgradePickUp::UpdateProjectileSound()
{
	if (!laserGunProjectileSound)return;
	playerRef->SetProjectileSound(laserGunProjectileSound);
}

void ALaserGunUpgradePickUp::UpdateProjectileImpulse()
{

	APlayerProjectile* _projectileRef = playerRef->projectileToSpawn.GetDefaultObject();
	if (!_projectileRef)return;
	_projectileRef->SetImpulseSpeed(laserImpulseSpeed);
}

void ALaserGunUpgradePickUp::UpdatePlayerAttackSpeed()
{
	playerRef->SetAttackSpeed(laserGunAttackSpeed);
}



void ALaserGunUpgradePickUp::ItemAction()
{
	TriggerPlayerGunSwap();
	UpdatePlayerProjectileMesh();
	UpdateProjectileManaCost();
	UpdateProjectileSound();
	UpdateProjectileImpulse();
	UpdatePlayerAttackSpeed();
}



