//Copyright © 2023 Pluz21(TVL).All rights reserved.

#include "CustomGameMode.h"
#include "DragonCanvas/Actors/ProjectileManager.h"

void ACustomGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
	InitProjectileManager();

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


