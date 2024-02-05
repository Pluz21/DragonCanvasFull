//Copyright © 2023 Pluz21(TVL).All rights reserved.

#include "CustomGameMode.h"
#include "DragonCanvas/Actors/ProjectileManager.h"
#include "DragonCanvas/Actors/GunManager.h"

void ACustomGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
	InitProjectileManager();
	InitGunManager();
}

void ACustomGameMode::InitProjectileManager()
{
	if (projectileManager)return; // SINGLETON 
	projectileManager = GetWorld()->SpawnActor<AProjectileManager>(projectileManagerToSpawn);
	if (!projectileManager)
	{
		UE_LOG(LogTemp, Warning, TEXT(" Failed to spawn ProjectileManager"));

	}
}

void ACustomGameMode::InitGunManager()
{
	if (gunManager)return;
	gunManager = GetWorld()->SpawnActor<AGunManager>(gunManagerToSpawn);
	if (!gunManager)
	{
		UE_LOG(LogTemp, Warning, TEXT(" Failed to spawn GunManager"));

	}
}


