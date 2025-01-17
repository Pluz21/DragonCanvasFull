// Copyright © 2023 Pluz21 (TVL). All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DragonCanvas/Actors/Gun.h"
#include "BaseGun.h"
#include "LaserGun.h"
#include "GunManager.generated.h"

UCLASS()
class DRAGONCANVAS_API AGunManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGunManager();

	UPROPERTY(EditAnywhere)
	TObjectPtr<ADragon> playerRef;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<AGun> currentGun = nullptr;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<ABaseGun> baseGunRef = nullptr;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<ALaserGun> laserGunRef = nullptr;

	UPROPERTY(VisibleAnywhere)
	TArray<AGun*> allGuns;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void Init();
	void AddItem(TObjectPtr<AGun> _item);
	void RemoveItem(TObjectPtr<AGun> _item);

	bool Exists(TObjectPtr<AGun> _item);
	bool Exists(const int& _index);

	TObjectPtr<AGun> GetGun(); //we need the index. 

};
