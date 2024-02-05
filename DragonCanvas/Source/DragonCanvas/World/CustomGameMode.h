//Copyright © 2023 Pluz21(TVL).All rights reserved.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DragonCanvas/Actors/ProjectileManager.h"
#include "CustomGameMode.generated.h"

/**
 * 
 */
class AProjectileManager;
class AGunManager;

UCLASS()
class DRAGONCANVAS_API ACustomGameMode : public AGameModeBase
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere)
	TSubclassOf<AProjectileManager> projectileManagerToSpawn;
	UPROPERTY(EditAnywhere)
	TSubclassOf<AGunManager> gunManagerToSpawn;
public :
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<AProjectileManager> projectileManager = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<AGunManager> gunManager = nullptr;

	
public :
	UFUNCTION(BlueprintCallable, BlueprintPure)
	AProjectileManager* GetProjectileManager() { return projectileManager; }
	UFUNCTION(BlueprintCallable, BlueprintPure)
	AGunManager* GetGunManager() { return gunManager; }

protected:

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	void InitProjectileManager();
	void InitGunManager();

};
