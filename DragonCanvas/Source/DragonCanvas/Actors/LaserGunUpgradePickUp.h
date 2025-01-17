// Copyright © 2023 Pluz21 (TVL). All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UpgradePickUpItem.h"
#include "LaserGunUpgradePickUp.generated.h"


UCLASS()
class DRAGONCANVAS_API ALaserGunUpgradePickUp : public AUpgradePickUpItem
{
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUpdateMeshEvent, UStaticMeshComponent*, _staticMesh);
	UPROPERTY()
	FUpdateMeshEvent onMeshUpdated;

	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALaserGunUpgradePickUp();

	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMesh> laserMesh;
	UPROPERTY(EditAnywhere)
	float laserProjectileManaCost = 0;
	UPROPERTY(EditAnywhere)
	float laserImpulseSpeed= 15000;
	UPROPERTY(EditAnywhere)
	float laserGunAttackSpeed = 10000;
	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> laserGunProjectileSound;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION() void TriggerPlayerGunSwap();
	UFUNCTION() virtual void UpdatePlayerProjectileMesh();
	UFUNCTION() virtual void UpdateProjectileManaCost();
	UFUNCTION() virtual void UpdateProjectileSound();
	UFUNCTION() virtual void UpdateProjectileImpulse();
	UFUNCTION() virtual void UpdatePlayerAttackSpeed();
	

	void ItemAction() override;
};
